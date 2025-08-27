import { Pipe, PipeTransform } from '@angular/core';

@Pipe({
    name: 'fileSize',
    standalone: false,
})
export class FileSizePipe implements PipeTransform {
    transform(value: number | undefined) {
        if (value === undefined) return '';

        if (value < 1024) return `${value} bytes`;
        if (value < 1024 * 1024) return `${(value / 1024).toFixed(1)} kB`;
        return `${(value / 1024 / 1024).toFixed(1)} MB`;
    }
}
