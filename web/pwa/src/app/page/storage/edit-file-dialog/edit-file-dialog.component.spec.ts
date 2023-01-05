import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EditFileDialogComponent } from './edit-file-dialog.component';

describe('EditFileDialogComponent', () => {
  let component: EditFileDialogComponent;
  let fixture: ComponentFixture<EditFileDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EditFileDialogComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(EditFileDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
