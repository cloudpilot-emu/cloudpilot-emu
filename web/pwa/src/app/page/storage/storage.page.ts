import { Component, ViewChild } from '@angular/core';

import { IonNav } from '@ionic/angular';
import { StorageCard } from '@pwa/model/StorageCard';
import { SubpageCardsComponent } from './subpage-cards/subpage-cards.component';
import { SubpageDirectoryComponent } from './subpage-directory/subpage-directory.component';

@Component({
    selector: 'app-storage',
    templateUrl: './storage.page.html',
    styleUrls: ['./storage.page.scss'],
})
export class StoragePage {
    readonly navRoot = SubpageCardsComponent;

    readonly navRootProps = {
        onMountForBrowse: (card: StorageCard) => this.browseCard(card),
    };

    private browseCard(card: StorageCard): void {
        this.nav?.push(SubpageDirectoryComponent, { card });
    }

    @ViewChild('nav')
    nav: IonNav | undefined;
}
