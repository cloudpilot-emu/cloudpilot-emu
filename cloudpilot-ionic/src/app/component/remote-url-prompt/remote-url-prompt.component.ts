import { AbstractControl, FormControl, FormGroup, ValidationErrors } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';

import Url from 'url-parse';

function validateUrl(control: AbstractControl): ValidationErrors | null {
    if (!control.value) {
        return {
            url: 'URL empty',
        };
    }

    let urlParsed: Url<unknown>;
    try {
        urlParsed = new Url(control.value);
    } catch (e) {
        return {
            url: 'URL invalid',
        };
    }

    if (location.protocol === 'https:' && urlParsed.protocol !== 'https:') {
        return {
            url: 'mixed content',
        };
    }

    return null;
}

@Component({
    selector: 'app-remote-url-prompt',
    templateUrl: './remote-url-prompt.component.html',
    styleUrls: ['./remote-url-prompt.component.scss'],
})
export class RemoteUrlPromptComponent implements OnInit {
    constructor() {}

    ngOnInit() {
        this.createFormGroup();
    }

    formKeyDown(e: KeyboardEvent): void {
        if (e.key === 'Enter') this.continue();
    }

    continue(): void {
        this.onContinue(new Url(this.formGroup.get('url')?.value).toString());
    }

    get isMixedContent(): boolean {
        const url = this.formGroup.get('url')?.value;
        if (!url) return false;

        try {
            const urlParsed = new Url(url);

            return location.protocol === 'https:' && urlParsed.protocol !== 'https:';
        } catch (e) {}

        return false;
    }

    private createFormGroup(): void {
        this.formGroup = new FormGroup({
            url: new FormControl('', validateUrl),
        });
    }

    @Input() onContinue: (url: string) => void = () => undefined;
    @Input() onCancel: () => void = () => undefined;

    formGroup!: FormGroup;
}
