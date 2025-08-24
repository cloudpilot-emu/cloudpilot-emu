import { ChangeDetectionStrategy, Component, EventEmitter, Input, Output, ViewChild } from '@angular/core';
import { IonItemSliding, PopoverController } from '@ionic/angular';

import { ContextMenuComponent } from '../context-menu/context-menu.component';
import { Session } from '@pwa/model/Session';
import { decodeVersion } from '@pwa/model/OSVersion';
import { deviceName } from '@common/helper/deviceProperties';

@Component({
    selector: 'app-session-item',
    templateUrl: './session-item.component.html',
    styleUrls: ['./session-item.component.scss'],
    changeDetection: ChangeDetectionStrategy.OnPush,
    standalone: false
})
export class SessionItemComponent {
    constructor(private popoverController: PopoverController) {}

    get device(): string {
        return this.session ? deviceName(this.session.device) : '';
    }

    async onContextmenu(e: MouseEvent): Promise<void> {
        e.stopPropagation();
        e.preventDefault();

        this.interaction.emit();

        const popover = await this.popoverController.create({
            component: ContextMenuComponent,
            event: e,
            arrow: false,
            dismissOnSelect: true,
            componentProps: {
                onEdit: () => this.edit.emit(),
                onSave: () => this.save.emit(),
                onDelete: () => this.delete.emit(),
                onReset: () => this.resetSession.emit(),
            },
            reference: 'event',
        });

        void popover.present();
    }

    decodeVersion(v: number | undefined): string {
        if (v === undefined) return '';

        const version = decodeVersion(v);

        return `${version.major}.${version.minor}`;
    }

    get color(): string | undefined {
        if (this.selected) return 'primary';
        if (this.touched) return 'light';

        return undefined;
    }

    @Input()
    session: Session | undefined;

    @Input()
    selected = false;

    @Input()
    touched = false;

    @Input()
    selecting = false;

    @Input()
    selectedForExport = false;

    @Output()
    delete = new EventEmitter<Session>();

    @Output()
    resetSession = new EventEmitter<Session>();

    @Output()
    edit = new EventEmitter<Session>();

    @Output()
    save = new EventEmitter<Session>();

    @Output()
    selectItem = new EventEmitter<Session>();

    @Output()
    interaction = new EventEmitter<Session>();

    @ViewChild('slidingItem')
    slidingItem!: IonItemSliding;
}
