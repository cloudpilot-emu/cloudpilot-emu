export function disambiguateName(originalName: string, isTaken: (name: string) => boolean): string {
    let disambiguatedName = originalName;

    let i = 1;
    while (isTaken(disambiguatedName)) {
        disambiguatedName = `${originalName} (${i++})`;
    }

    return disambiguatedName;
}
