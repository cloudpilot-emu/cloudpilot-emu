import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { EmulationPage } from './emulation.page';

describe('EmulationPage', () => {
  let component: EmulationPage;
  let fixture: ComponentFixture<EmulationPage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ EmulationPage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(EmulationPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
