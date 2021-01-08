import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { SessionItemComponent } from './session-item.component';

describe('SessionItemComponent', () => {
  let component: SessionItemComponent;
  let fixture: ComponentFixture<SessionItemComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ SessionItemComponent ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(SessionItemComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
