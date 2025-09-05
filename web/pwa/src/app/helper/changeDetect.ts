import { ChangeDetectorRef } from '@angular/core';

export function changeDetector<T>(cd: ChangeDetectorRef, oldValue: T, updateFn: () => T): () => void {
    return () => {
        const newValue = updateFn();

        if (newValue !== oldValue) {
            cd.markForCheck();
            oldValue = newValue;
        }
    };
}
