import { Component } from '@angular/core';
import { SessionsPage } from '../sessions.page';

// We want to use onPush change detection for the sessions page, and this does not work
// if the actual page is top level. Using <ion-nav> makes sure that the page is displayed
// correctly.

@Component({
    selector: 'app-sessions-wrapper',
    templateUrl: './nav-wrapper.component.html',
})
export class NavWrapperComponent {
    readonly navRoot = SessionsPage;
}
