import { Component, Input, OnInit } from '@angular/core';
import { FormControl, FormGroup } from '@angular/forms';

import { KvsService } from '@pwa/service/kvs.service';

@Component({
    selector: 'app-credentials-prompt',
    templateUrl: './credentials-prompt.component.html',
    styleUrls: ['./credentials-prompt.component.scss'],
})
export class CredentialsPromptComponent implements OnInit {
    constructor(private kvs: KvsService) {}

    ngOnInit() {
        this.createFormGroup();
    }

    formKeyDown(e: KeyboardEvent): void {
        if (e.key === 'Enter') this.saveAndContinue();
    }

    saveAndContinue(): void {
        this.kvs.kvs.credentials = {
            ...this.kvs.kvs.credentials,
            [this.origin]: {
                username: this.formGroup.get('username')?.value ?? '',
                password: this.formGroup.get('password')?.value ?? '',
            },
        };

        this.onContinue();
    }

    private createFormGroup(): void {
        const auth = this.kvs.kvs.credentials[this.origin];

        this.formGroup = new FormGroup({
            username: new FormControl(auth?.username ?? ''),
            password: new FormControl(auth?.password ?? ''),
        });
    }

    @Input()
    onContinue: () => void = () => undefined;

    @Input()
    onCancel: () => void = () => undefined;

    @Input()
    origin = '';

    formGroup!: FormGroup;
}
