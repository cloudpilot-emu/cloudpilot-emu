import { Component, EventEmitter, Input, Output } from '@angular/core';
import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { ContextMenuComponent } from '../context-menu/context-menu.component';
import { PopoverController } from '@ionic/angular';
import { StorageCardService } from './../../../service/storage-card.service';

@Component({
    selector: 'app-storage-card',
    templateUrl: './storage-card.component.html',
    styleUrls: ['./storage-card.component.scss'],
})
export class StorageCardComponent {
    constructor(private storageCardService: StorageCardService, private popoverController: PopoverController) {}

    async onContextmenu(e: MouseEvent): Promise<void> {
        e.stopPropagation();
        e.preventDefault();

        this.interaction.emit();

        const popover = await this.popoverController.create({
            component: ContextMenuComponent,
            event: e,
            backdropDismiss: true,
            showBackdrop: false,
            componentProps: {
                isAttached: this.isAttached,
                onEdit: () => {
                    popover.dismiss();
                    this.edit.emit(this.card);
                },
                onSave: () => {
                    popover.dismiss();
                    this.save.emit(this.card);
                },
                onCheck: () => {
                    popover.dismiss();
                    this.check.emit();
                },
                onEject: () => {
                    popover.dismiss();
                    this.eject.emit();
                },
                onDelete: () => {
                    popover.dismiss();
                    this.delete.emit();
                },
            },
        });

        popover.present();
    }

    get color(): string | undefined {
        if (this.touched) return 'light';

        return undefined;
    }

    get status(): string {
        if (!this.card) {
            return '';
        }

        const session = this.storageCardService.mountedInSession(this.card?.id);

        if (session) {
            return `attached to '${session.name}'`;
        }

        switch (this.card.status) {
            case StorageCardStatus.dirty:
                return 'not checked';

            case StorageCardStatus.clean:
                return 'clean';

            default:
                throw new Error('unreachable');
        }
    }

    get isAttached(): boolean {
        return this.card !== undefined && !!this.storageCardService.mountedInSession(this.card.id);
    }

    @Input()
    card: StorageCard | undefined;

    @Input()
    touched = false;

    @Output()
    interaction = new EventEmitter<StorageCard>();

    @Output()
    select = new EventEmitter<StorageCard>();

    @Output()
    edit = new EventEmitter<StorageCard>();

    @Output()
    check = new EventEmitter<StorageCard>();

    @Output()
    eject = new EventEmitter<StorageCard>();

    @Output()
    delete = new EventEmitter<StorageCard>();

    @Output()
    save = new EventEmitter<StorageCard>();
}
