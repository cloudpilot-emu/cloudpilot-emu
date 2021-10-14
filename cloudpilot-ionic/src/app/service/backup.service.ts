import { concatFilenames, filenameForBackup } from 'src/app/helper/filename';

import { AlertService } from './alert.service';
import { EmulationService } from './emulation.service';
import { EmulationStateService } from './emulation-state.service';
import { FileService } from 'src/app/service/file.service';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { SnapshotService } from './snapshot.service';

@Injectable({ providedIn: 'root' })
export class BackupService {
    constructor(
        private emulationService: EmulationService,
        private loadingController: LoadingController,
        private snapshotService: SnapshotService,
        private fileService: FileService,
        private emulationStateService: EmulationStateService,
        private alertService: AlertService
    ) {}

    async saveBackup(includeRomDatabases: boolean): Promise<void> {
        const currentSession = this.emulationStateService.getCurrentSession();
        if (!currentSession) return;

        const loader = await this.loadingController.create();
        await loader.present();

        let failedDatabases: Array<string> = [];
        let archive: Uint8Array | undefined;

        await this.emulationService.pause();
        await this.snapshotService.waitForPendingSnapshot();

        try {
            const cloudpilot = await this.emulationService.cloudpilot;

            await cloudpilot.backup(async (dbBackup) => {
                dbBackup.Init(includeRomDatabases);

                while (dbBackup.IsInProgress()) {
                    const db = dbBackup.GetCurrentDatabase();

                    if (!dbBackup.Save()) failedDatabases.push(db);

                    await new Promise((r) => setTimeout(r, 0));
                }

                archive = cloudpilot.getArchive(dbBackup);
            });
        } finally {
            await loader.dismiss();
            await this.emulationService.resume();
        }

        if (!archive) {
            await this.alertService.errorMessage('Backup failed.');
        } else if (failedDatabases.length > 0) {
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
                'Cancel'
            );
        } else {
            this.fileService.saveFile(filenameForBackup(currentSession, includeRomDatabases), archive);
        }
    }
}
