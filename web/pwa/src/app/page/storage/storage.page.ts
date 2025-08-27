import { Component, OnInit, ViewChild } from '@angular/core';

import { DragDropClient } from '@pwa/service/drag-drop.service';
import { DragDropService } from '@pwa//service/drag-drop.service';
import { IonNav } from '@ionic/angular';
import { StorageCard } from '@pwa/model/StorageCard';
import { SubpageCardsComponent } from './subpage-cards/subpage-cards.component';
import { SubpageDirectoryComponent } from './subpage-directory/subpage-directory.component';
import { VfsService } from '@pwa/service/vfs.service';
import { isFirefox } from '@common/helper/browser';

@Component({
    selector: 'app-storage',
    templateUrl: './storage.page.html',
    styleUrls: ['./storage.page.scss'],
    standalone: false,
})
export class StoragePage implements OnInit, DragDropClient {
    constructor(
        private vfsService: VfsService,
        private dragDropService: DragDropService,
    ) {}

    ngOnInit(): void {
        this.vfsService.onReleaseCard.addHandler(this.onReleaseCard.bind(this));
    }

    async ionViewDidLeave(): Promise<void> {
        const currentView = await this.nav?.getActive();

        currentView?.params?.selfReference?.ref?.ionViewDidLeave?.();
    }

    ionViewDidEnter(): void {
        this.isVisible = true;
        this.dragDropService.registerClient(this);
    }

    ionViewWillLeave(): void {
        this.isVisible = false;
        this.dragDropService.unregisterClient(this);
    }

    async handleDragDropEvent(e: DragEvent): Promise<void> {
        const currentView = await this.nav?.getActive();

        await currentView?.params?.selfReference?.ref?.handleDragDropEvent?.(e);
    }

    readonly navRootProps = {
        onMountForBrowse: (card: StorageCard) => this.browseCard(card),
        selfReference: {},
    };

    get animateNav(): boolean {
        return !isFirefox;
    }

    private browseCard(card: StorageCard): void {
        this.currentlyBrowsingCard = card;
        void this.nav?.push(
            SubpageDirectoryComponent,
            {
                card,
                path: '/',
                onNavigate: this.onNavigate,
                onNavigateBreadcrumb: this.onNavigateBreadcrumb,
                selfReference: {},
            },
            { animated: this.animateNav },
        );
    }

    private onNavigate = (path: string): void => {
        if (!this.currentlyBrowsingCard) return;

        void this.nav?.push(
            SubpageDirectoryComponent,
            {
                card: this.currentlyBrowsingCard,
                path,
                onNavigate: this.onNavigate,
                onNavigateBreadcrumb: this.onNavigateBreadcrumb,
                selfReference: {},
            },
            { animated: this.animateNav },
        );
    };

    private onNavigateBreadcrumb = (index: number): void => {
        if (!this.currentlyBrowsingCard) return;

        void this.nav?.popTo(index, { animated: this.animateNav });
    };

    private onReleaseCard(id: number): void {
        if (id !== this.currentlyBrowsingCard?.id || !this.nav) return;

        if (!this.isVisible) this.nav.animated = false;
        void this.nav.popToRoot(undefined, () => this.nav && (this.nav.animated = this.animateNav));
    }

    @ViewChild('nav')
    nav: IonNav | undefined;

    readonly navRoot = SubpageCardsComponent;

    private currentlyBrowsingCard: StorageCard | undefined;
    private isVisible = false;
}
