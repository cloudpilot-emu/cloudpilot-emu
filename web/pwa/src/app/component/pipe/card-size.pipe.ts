import { Pipe, PipeTransform } from '@angular/core';

@Pipe({ name: 'cardSize' })
export class CardSizePipe implements PipeTransform {
    transform(value: number | undefined) {
        return value !== undefined ? `${Math.round(value / 1024 / 1024)} MB` : '';
    }
}
