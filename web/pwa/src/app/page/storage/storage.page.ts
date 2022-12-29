import { Component, OnInit, ViewChild } from '@angular/core';

import { IonNav } from '@ionic/angular';
import { StorageCard } from '@pwa/model/StorageCard';
import { SubpageCardsComponent } from './subpage-cards/subpage-cards.component';
import { SubpageDirectoryComponent } from './subpage-directory/subpage-directory.component';
import { VfsService } from '@pwa/service/vfs.service';

@Component({
    selector: 'app-storage',
    templateUrl: './storage.page.html',
    styleUrls: ['./storage.page.scss'],
})
export class StoragePage implements OnInit {
    constructor(private vfsService: VfsService) {}

    ngOnInit(): void {
        this.vfsService.onReleaseCard.addHandler(
            (id) => id === this.currentlyBrowsingCard?.id && this.nav?.popToRoot()
        );
    }

    readonly navRootProps = {
        onMountForBrowse: (card: StorageCard) => this.browseCard(card),
    };

    private browseCard(card: StorageCard): void {
        this.currentlyBrowsingCard = card;
        this.nav?.push(SubpageDirectoryComponent, { card, path: '/', onNavigate: this.onNavigate });
    }

    private onNavigate = (path: string): void => {
        if (!this.currentlyBrowsingCard) return;

        this.nav?.push(SubpageDirectoryComponent, {
            card: this.currentlyBrowsingCard,
            path,
            onNavigate: this.onNavigate,
        });
    };

    @ViewChild('nav')
    nav: IonNav | undefined;

    readonly navRoot = SubpageCardsComponent;

    private currentlyBrowsingCard: StorageCard | undefined;
}
