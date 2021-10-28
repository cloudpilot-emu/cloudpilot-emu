import { AlertController, LoadingController } from '@ionic/angular';

import { AlertService } from './alert.service';
import { DbInstallResult } from '../helper/Cloudpilot';
import { EmulationService } from './emulation.service';
import { FileDescriptor } from './file.service';
import { Injectable } from '@angular/core';
import { JobQueue } from './../helper/JobQueue';
import { SnapshotService } from './snapshot.service';
import { ZipfileWalkerState } from '../../../../src';
import { concatFilenames } from '../helper/filename';

const ZIP_SIZE_LIMIT = 32 * 1024 * 1024;
const SNAPSHOT_LIMIT = 4 * 1024 * 1024;

function isInstallable(filename: string) {
    return /\.(prc|pdb|pqa)$/i.test(filename);
}

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

class InstallationContext {
    constructor(
        private emulationService: EmulationService,
        private alertController: AlertController,
        private snapshotService: SnapshotService,
        private files: Array<FileDescriptor>
    ) {}

    async run(): Promise<[Array<string>, Array<string>, Array<string>]> {
        for (const file of this.files) {
            if (/\.zip/i.test(file.name) && file.content.length < ZIP_SIZE_LIMIT) {
                try {
                    await this.installZip(file);
                } catch (err) {
                    this.filesFail.push(file.name);
                }
            } else if (isInstallable(file.name)) {
                await this.installOne(file.name, file.content);
            } else {
                this.filesFail.push(file.name);
            }
        }

        await this.errorMessageQueue.isClear();

        return [this.filesSuccess, this.filesRequireReset, this.filesFail];
    }

    private async installOne(name: string, content: Uint8Array): Promise<void> {
        const cloudpilot = await this.emulationService.cloudpilot;
        const code = await cloudpilot.installDb(content);

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

            default:
                this.filesFail.push(name);
                this.errorMessageQueue.push(() => this.reportError(name, code));

                break;
        }

        if (this.sizeInstalledSinceLastsnapshot > SNAPSHOT_LIMIT) {
            this.snapshotService.triggerSnapshot();
            this.sizeInstalledSinceLastsnapshot = 0;
        }

        await new Promise((r) => setTimeout(r, 0));
    }

    private async installZip(file: FileDescriptor): Promise<void> {
        const cloudpilot = await this.emulationService.cloudpilot;

        await cloudpilot.withZipfileWalker(file.content, async (walker) => {
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
        });
    }

    private reportError(file: string, code: DbInstallResult): Promise<void> {
        if (this.skipErrors) return Promise.resolve();

        return new Promise((resolve) => {
            const alert = this.alertController.create({
                header: 'Installation failed',
                backdropDismiss: false,
                cssClass: 'alert-checkbox-no-border installation-error',
                message: `Could not install ${file}: ${describeError(code)}.`,
                buttons: [
                    {
                        text: 'Close',
                        role: 'cancel',
                        handler: () => {
                            this.alertController.dismiss();
                            resolve();
                        },
                    },
                ],
                inputs:
                    this.filesFail.length >= 3
                        ? [
                              {
                                  type: 'checkbox',
                                  label: 'Skip any remaining errors',
                                  checked: false,
                                  handler: (inpt) => (this.skipErrors = inpt.checked === true),
                              },
                          ]
                        : [],
            });

            alert.then((a) => a.present());
        });
    }

    private filesSuccess: Array<string> = [];
    private filesFail: Array<string> = [];
    private filesRequireReset: Array<string> = [];
    private errorMessageQueue = new JobQueue();

    private sizeInstalledSinceLastsnapshot = 0;
    private skipErrors = false;
}

@Injectable({ providedIn: 'root' })
export class InstallationService {
    constructor(
        private emulationService: EmulationService,
        private loadingController: LoadingController,
        private snapshotService: SnapshotService,
        private alertService: AlertService,
        private alertController: AlertController
    ) {}

    async installFiles(files: Array<FileDescriptor>): Promise<void> {
        const loader = await this.loadingController.create({
            message: 'Installing...',
        });

        await loader.present();
        await this.emulationService.pause();
        await this.snapshotService.waitForPendingSnapshot();

        let filesSuccess: Array<string> = [];
        let filesFail: Array<string> = [];
        let filesRequireReset: Array<string> = [];

        try {
            const installationContext = new InstallationContext(
                this.emulationService,
                this.alertController,
                this.snapshotService,
                files
            );
            [filesSuccess, filesRequireReset, filesFail] = await installationContext.run();
        } finally {
            loader.dismiss();

            this.emulationService.resume();
        }

        const message = [
            this.messageSucces(filesSuccess),
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

        this.alertService.message(
            header,
            message,
            filesRequireReset.length > 0 ? { 'Reset now': () => this.emulationService.reset() } : undefined
        );
    }

    private messageSucces(filesSuccess: Array<string>): string | undefined {
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
