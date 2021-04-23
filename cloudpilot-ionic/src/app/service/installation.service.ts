import { AlertService } from './alert.service';
import { DbInstallResult } from '../helper/Cloudpilot';
import { EmulationService } from './emulation.service';
import { EmulationStateService } from './emulation-state.service';
import { FileDescriptor } from './file.service';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { SnapshotService } from './snapshot.service';
import { concatFilenames } from '../helper/filename';

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

        const filesSuccess: Array<string> = [];
        const filesFail: Array<string> = [];
        const filesRequireReset: Array<string> = [];

        try {
            for (const file of files) {
                if (!/\.(prc|pdb|pqa)$/i.test(file.name)) {
                    filesFail.push(file.name);

                    continue;
                }

                switch (await this.emulationService.installDb(file.content)) {
                    case DbInstallResult.failure:
                        filesFail.push(file.name);
                        break;

                    case DbInstallResult.needsReboot:
                        filesRequireReset.push(file.name);
                        filesSuccess.push(file.name);
                        break;

                    case DbInstallResult.success:
                        filesSuccess.push(file.name);
                        break;
                }

                await new Promise((r) => setTimeout(r, 0));
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
                return `Installation of ${filesSuccess.length} files successful.`;
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
