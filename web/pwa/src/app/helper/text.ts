export function ucFirst(input: string): string {
    return input.length === 0 ? input : input[0].toUpperCase() + input.substring(1);
}
