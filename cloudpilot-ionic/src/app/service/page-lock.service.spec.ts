import { TestBed } from '@angular/core/testing';

import { PageLockService } from './page-lock.service';

describe('PageLockService', () => {
  let service: PageLockService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(PageLockService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
