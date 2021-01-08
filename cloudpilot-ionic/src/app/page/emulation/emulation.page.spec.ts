import { ComponentFixture, TestBed, async } from '@angular/core/testing';

import { EmulationPage } from './emulation.page';
import { IonicModule } from '@ionic/angular';

describe('EmulationPage', () => {
    let component: EmulationPage;
    let fixture: ComponentFixture<EmulationPage>;

    beforeEach(async(() => {
        TestBed.configureTestingModule({
            declarations: [EmulationPage],
            imports: [IonicModule.forRoot()],
        }).compileComponents();

        fixture = TestBed.createComponent(EmulationPage);
        component = fixture.componentInstance;
        fixture.detectChanges();
    }));

    it('should create', () => {
        expect(component).toBeTruthy();
    });
});
