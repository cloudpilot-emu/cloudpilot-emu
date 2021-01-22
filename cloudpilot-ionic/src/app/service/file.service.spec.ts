import { FileService } from './file.service';
import { TestBed } from '@angular/core/testing';

describe('FileService', () => {
    let service: FileService;

    beforeEach(() => {
        TestBed.configureTestingModule({});
        service = TestBed.inject(FileService);
    });

    it('should be created', () => {
        expect(service).toBeTruthy();
    });
});
