import { TestBed } from '@angular/core/testing';

import { EmulationService } from './emulation.service';

describe('EmulationService', () => {
  let service: EmulationService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(EmulationService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
