import { Inject, Injectable } from '@angular/core';
import { DbInstallResult } from '@common/bridge/Cloudpilot';
import { FsTools } from '@common/bridge/FSTools';
import { ZipfileWalkerState } from '@common/bridge/ZipfileWalker';

import { concatFilenames } from '@pwa/helper/filename';
import { Lock } from '@pwa/helper/lock';

import { AlertService } from './alert.service';
import { EmulationContextService } from './emulation-context.service';
import { EmulationService } from './emulation.service';
import { FileDescriptor } from './file.service';
import { FsToolsService } from './fstools.service';
import { LoaderService } from './loader.service';
import { SnapshotService } from './snapshot.service';
import { TOKEN_EMULATOR_LOCK } from './token';

const ZIP_SIZE_LIMIT = 32 * 1024 * 1024;
const SNAPSHOT_LIMIT = 4 * 1024 * 1024;

function isInstallable(filename: string) {
    return /\.(prc|pdb|pqa)$/i.test(filename);
}

class InstallationNotPossibleError extends Error {}

function describeError(code: DbInstallResult): string {
    switch (code) {
        case DbInstallResult.failedCouldNotOverwrite:
            return 'database exists on device and could not be overwritten';

        case DbInstallResult.failureDbIsCorrupt:
            return 'database file is corrupt';

        case DbInstallResult.failureDbIsOpen:
            return 'database is open on device';

        case DbInstallResult.failureInternal:
            return 'internal PalmOS error';

        case DbInstallResult.failureNotEnoughMemory:
            return 'not enough memory on device';

        default:
            return 'unknown reason';
    }
}

function getDatabaseName(data: Uint8Array): string {
    if (data.length < 32) throw new Error('not a database');

    let iTerminator: number;
    for (iTerminator = 0; iTerminator < 32; iTerminator++) {
        if (data[iTerminator] === 0) break;
    }

    if (iTerminator > 31) throw new Error('not a database');

    return new TextDecoder('latin1', { fatal: false }).decode(data.subarray(0, iTerminator));
}

class InstallationContext {
    constructor(
        private fstools: FsTools,
        private alertService: AlertService,
        private snapshotService: SnapshotService,
        private files: Array<FileDescriptor>,
        private emulationContext: EmulationContextService,
    ) {}

    async run(): Promise<[Array<string>, Array<string>, Array<string>]> {
        for (const file of this.files) {
            let content: Uint8Array;
            try {
                content = await file.getContent();
            } catch (e) {
                console.warn(e);

                this.filesFail.push(file.name);
                continue;
            }

            if (/\.zip/i.test(file.name) && content.length < ZIP_SIZE_LIMIT) {
                try {
                    await this.installZip(file);
                    // eslint-disable-next-line @typescript-eslint/no-unused-vars
                } catch (e) {
                    this.filesFail.push(file.name);
                }
            } else if (isInstallable(file.name)) {
                await this.installOne(file.name, content);
            } else {
                this.filesFail.push(file.name);
            }
        }

        return [this.filesSuccess, this.filesRequireReset, this.filesFail];
    }

    private async installOne(name: string, content: Uint8Array): Promise<void> {
        const engine = await this.emulationContext.engine();
        if (!engine) throw new Error('emulator not running');

        const code = await engine.installDb(content);

        switch (code) {
            case DbInstallResult.needsReboot:
                this.filesRequireReset.push(name);
                this.filesSuccess.push(name);
                this.sizeInstalledSinceLastsnapshot += content.length;
                break;

            case DbInstallResult.success:
                this.filesSuccess.push(name);
                this.sizeInstalledSinceLastsnapshot += content.length;
                break;

            case DbInstallResult.failureInstallationNotPossible:
                throw new InstallationNotPossibleError('DB installation not currently possible');

            default:
                this.filesFail.push(name);
                await this.reportError(name, code);

                break;
        }

        if (code === DbInstallResult.needsReboot || code === DbInstallResult.success) {
            try {
                const dbName = getDatabaseName(content);

                if (this.installedDatabases.has(dbName)) {
                    await this.errorDialog(
                        'Duplicate item',
                        `${name} overwrites previously installed database ${this.installedDatabases.get(dbName)}`,
                    );
                }

                this.installedDatabases.set(dbName, name);
                // eslint-disable-next-line @typescript-eslint/no-unused-vars
            } catch (e) {
                // ignore stuff that is not a database
            }
        }

        if (this.sizeInstalledSinceLastsnapshot > SNAPSHOT_LIMIT) {
            await this.snapshotService.triggerSnapshot();
            this.sizeInstalledSinceLastsnapshot = 0;
        }

        await new Promise((r) => setTimeout(r, 0));
    }

    private async installZip(file: FileDescriptor): Promise<void> {
        const walker = await this.fstools.createZipfileWalker(await file.getContent());

        let installed = 0;

        while (walker.GetState() === ZipfileWalkerState.open) {
            const name = walker.GetCurrentEntryName().replace(/^.*\//, '');

            if (!isInstallable(walker.GetCurrentEntryName())) {
                walker.Next();
                continue;
            }
            const content = walker.GetCurrentEntryContent();
            if (content) {
                await this.installOne(name, content);
                installed++;
            } else {
                this.filesFail.push(name);
            }

            walker.Next();
        }

        if (installed === 0) throw new Error('no installable files in archive');
    }

    private reportError(file: string, code: DbInstallResult): Promise<void> {
        return this.errorDialog('Item failed to install', `Could not install ${file}: ${describeError(code)}.`);
    }

    private errorDialog(header: string, message: string): Promise<void> {
        if (this.skipErrors) return Promise.resolve();

        return this.alertService
            .messageWithChoice(header, message, 'Skip any remaining errors', false, {}, 'Continue')
            .then((choice) => void (this.skipErrors = choice));
    }

    private filesSuccess: Array<string> = [];
    private filesFail: Array<string> = [];
    private filesRequireReset: Array<string> = [];

    private sizeInstalledSinceLastsnapshot = 0;
    private skipErrors = false;

    private installedDatabases = new Map<string, string>();
}

@Injectable({ providedIn: 'root' })
export class InstallationService {
    constructor(
        private emulationService: EmulationService,
        private loaderService: LoaderService,
        private snapshotService: SnapshotService,
        private alertService: AlertService,
        private fstools: FsToolsService,
        private emulationContext: EmulationContextService,
        @Inject(TOKEN_EMULATOR_LOCK) private emulatorLock: Lock,
    ) {}

    async installFiles(files: Array<FileDescriptor>): Promise<void> {
        try {
            await this.installFilesUnguarded(files);
        } catch (e) {
            if (e instanceof InstallationNotPossibleError) {
                void this.alertService.errorMessage(
                    'Installation is not currently possible. Navigate to the PalmOS launcher and try again.',
                );
                return;
            }

            throw e;
        }
    }

    private async installFilesUnguarded(files: Array<FileDescriptor>): Promise<void> {
        const [filesSuccess, filesRequireReset, filesFail] = await this.emulatorLock.runGuarded(async () => {
            return this.loaderService.showWhile(async () => {
                await this.emulationService.flush();
                await this.emulationService.waitForPendingSnapshot();

                const installationContext = new InstallationContext(
                    this.fstools,
                    this.alertService,
                    this.snapshotService,
                    files,
                    this.emulationContext,
                );

                return await installationContext.run();
            }, 'Installing...');
        });

        const message = [
            this.messageSuccess(filesSuccess),
            this.messageFailed(filesFail),
            this.messageRequireReset(filesRequireReset),
        ]
            .filter((x) => !!x)
            .join('<br/><br/>');

        let header;

        if (filesFail.length === 0) {
            header = 'Installation successful';
        } else if (filesSuccess.length === 0) {
            header = 'Installation failed';
        } else {
            header = 'Installation errors';
        }

        void this.alertService.message(
            header,
            message,
            filesRequireReset.length > 0 ? { 'Reset now': () => void this.emulationService.reset() } : undefined,
        );
    }

    private messageSuccess(filesSuccess: Array<string>): string | undefined {
        switch (filesSuccess.length) {
            case 0:
                return undefined;

            case 1:
                return `Installation of ${filesSuccess[0]} successful.`;

            default:
                return `Installation of ${filesSuccess.length} items successful.`;
        }
    }

    private messageFailed(filesFail: Array<string>): string | undefined {
        return filesFail.length > 0 ? `Installation of  ${concatFilenames(filesFail)} failed.` : undefined;
    }

    private messageRequireReset(filesRequireReset: Array<string>): string | undefined {
        return filesRequireReset.length > 0
            ? `${concatFilenames(filesRequireReset)} require a device reset.`
            : undefined;
    }
}
