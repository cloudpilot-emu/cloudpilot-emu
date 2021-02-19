import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';

import { ContextMenuComponent } from './../context-menu/context-menu.component';
import { DeviceId } from 'src/app/model/DeviceId';
import { PopoverController } from '@ionic/angular';
import { Session } from './../../../model/Session';

@Component({
    selector: 'app-session-item',
    templateUrl: './session-item.component.html',
    styleUrls: ['./session-item.component.scss'],
})
export class SessionItemComponent {
    constructor(private popoverController: PopoverController) {}

    get device(): string {
        switch (this.session?.device) {
            case DeviceId.m515:
                return 'Palm m515';

            case DeviceId.palmV:
                return 'Palm V';

            default:
                throw new Error('bad device ID');
        }
    }

    async onContextmenu(e: MouseEvent): Promise<void> {
        e.stopPropagation();
        e.preventDefault();

        const popover = await this.popoverController.create({
            component: ContextMenuComponent,
            event: e,
            backdropDismiss: true,
            showBackdrop: false,
            componentProps: {
                onEdit: () => {
                    popover.dismiss();
                    this.edit.emit();
                },
                onSave: () => {
                    popover.dismiss();
                    this.save.emit();
                },
                onDelete: () => {
                    popover.dismiss();
                    this.delete.emit();
                },
            },
        });

        popover.present();
    }

    @Input()
    session: Session | undefined;

    @Input()
    selected = false;

    @Output()
    delete = new EventEmitter<Session>();

    @Output()
    edit = new EventEmitter<Session>();

    @Output()
    save = new EventEmitter<Session>();
}
