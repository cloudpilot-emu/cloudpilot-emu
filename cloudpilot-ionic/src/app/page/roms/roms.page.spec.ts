import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { RomsPage } from './roms.page';

describe('RomsPage', () => {
  let component: RomsPage;
  let fixture: ComponentFixture<RomsPage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ RomsPage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(RomsPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
