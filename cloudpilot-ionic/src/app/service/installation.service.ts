import { AlertService } from './alert.service';
import { DbInstallResult } from '../helper/Cloudpilot';
import { EmulationService } from './emulation.service';
import { EmulationStateService } from './emulation-state.service';
import { FileDescriptor } from './file.service';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { SnapshotService } from './snapshot.service';
import { ZipfileWalkerState } from '../../../../src';
import { concatFilenames } from '../helper/filename';

const ZIP_SIZE_LIMIT = 32 * 1024 * 1024;

function isInstallable(filename: string) {
    return /\.(prc|pdb|pqa)$/i.test(filename);
}

@Injectable({ providedIn: 'root' })
export class InstallationService {
    constructor(
        private emulationService: EmulationService,
        private loadingController: LoadingController,
        private snapshotService: SnapshotService,
        private emulationStateService: EmulationStateService,
        private alertService: AlertService
    ) {}

    async installFiles(files: Array<FileDescriptor>): Promise<void> {
        const loader = await this.loadingController.create({
            message: 'Installing...',
        });

        await loader.present();
        await this.emulationService.pause();
        await this.snapshotService.waitForPendingSnapshot();

        const cloudpilot = await this.emulationService.cloudpilot;

        const filesSuccess: Array<string> = [];
        const filesFail: Array<string> = [];
        const filesRequireReset: Array<string> = [];

        const installOne = async (name: string, content: Uint8Array) => {
            switch (await cloudpilot.installDb(content)) {
                case DbInstallResult.failure:
                    filesFail.push(name);
                    break;

                case DbInstallResult.needsReboot:
                    filesRequireReset.push(name);
                    filesSuccess.push(name);
                    break;

                case DbInstallResult.success:
                    filesSuccess.push(name);
                    break;
            }

            await new Promise((r) => setTimeout(r, 0));
        };

        try {
            for (const file of files) {
                if (/\.zip/i.test(file.name) && file.content.length < ZIP_SIZE_LIMIT) {
                    try {
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
                                    await installOne(name, content);
                                    installed++;
                                } else {
                                    filesFail.push(name);
                                }

                                walker.Next();
                            }

                            if (installed === 0) throw new Error('no installable files in archive');
                        });
                    } catch (err) {
                        filesFail.push(file.name);
                    }
                } else if (isInstallable(file.name)) {
                    await installOne(file.name, file.content);
                } else {
                    filesFail.push(file.name);
                }
            }
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
