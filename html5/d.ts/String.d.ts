interface String {
    insertAt(startPos: number, str: string): string;
    removeAt(startPos: number, length: number): string;
    replaceAll(str: string, replacement: string): string;
}
