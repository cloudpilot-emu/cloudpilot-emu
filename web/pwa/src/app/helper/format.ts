const ONE_DIGIT_NUMBERS = ['zero', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine'];

export function formatNumeral(x: number): string {
    return x < 10 ? ONE_DIGIT_NUMBERS[x] : x + '';
}
