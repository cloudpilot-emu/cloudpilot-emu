import { Component, ViewChild } from '@angular/core';
import { IonNav } from '@ionic/angular';

import { SessionsPage } from '../sessions.page';

// We want to use onPush change detection for the sessions page, and this does not work
// if the actual page is top level. Using <ion-nav> makes sure that the page is displayed
// correctly.

@Component({
    selector: 'app-sessions-wrapper',
    templateUrl: './nav-wrapper.component.html',
    standalone: false,
})
export class NavWrapperComponent {
    async ionViewDidLeave(): Promise<void> {
        const currentView = await this.nav?.getActive();

        currentView?.params?.selfReference?.ref?.ionViewDidLeave();
    }

    async ionViewWillLeave(): Promise<void> {
        const currentView = await this.nav?.getActive();

        currentView?.params?.selfReference?.ref?.ionViewWillLeave();
    }

    async ionViewDidEnter(): Promise<void> {
        const currentView = await this.nav?.getActive();

        currentView?.params?.selfReference?.ref?.ionViewDidEnter_();
    }

    @ViewChild('nav')
    nav: IonNav | undefined;

    readonly navRoot = SessionsPage;
}
