import { Inject, Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';

import { concatFilenames, filenameForBackup } from '@pwa/helper/filename';
import { Lock } from '@pwa/helper/lock';

import { AlertService } from './alert.service';
import { EmulationContextService } from './emulation-context.service';
import { EmulationService } from './emulation.service';
import { FileService } from './file.service';
import { TOKEN_EMULATOR_LOCK } from './token';

@Injectable({ providedIn: 'root' })
export class BackupService {
    constructor(
        private loadingController: LoadingController,
        private emulationService: EmulationService,
        private fileService: FileService,
        private emulationContext: EmulationContextService,
        @Inject(TOKEN_EMULATOR_LOCK) private emulatorLock: Lock,
        private alertService: AlertService,
    ) {}

    async saveBackup(includeRomDatabases: boolean): Promise<void> {
        const currentSession = this.emulationContext.session();
        if (!currentSession) return;

        const loader = await this.loadingController.create({ message: 'Backing up...' });

        const { failedDatabases, archive } = await this.emulatorLock.runGuarded(async () => {
            await loader.present();
            await this.emulationService.flush();
            await this.emulationService.waitForPendingSnapshot();

            try {
                const engine = this.emulationContext.engine();
                if (!engine) throw new Error('emulator not running');

                return (await engine.backup(includeRomDatabases)) ?? { failedDatabases: undefined, archive: undefined };
            } finally {
                await loader.dismiss();
            }
        });

        if (!archive) {
            await this.alertService.errorMessage('Backup failed.');
        } else if (failedDatabases !== undefined && failedDatabases.length > 0) {
            await this.alertService.message(
                'Backup complete',
                `
                    ${concatFilenames(failedDatabases)} failed  to backup.
                    <br/><br/>
                    Open databases cannot be backuped. Please run backup from the launcher
                    in order to backup all databases.
                `,
                {
                    'Save Backup': () =>
                        this.fileService.saveFile(filenameForBackup(currentSession, includeRomDatabases), archive!),
                },
                'Cancel',
            );
        } else {
            this.fileService.saveFile(filenameForBackup(currentSession, includeRomDatabases), archive);
        }
    }
}
