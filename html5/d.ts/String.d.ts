interface String {
    insertAt(startPos: number, str: string): String;
    removeAt(startPos: number, length: number): String;
    replaceAll(str: string, replacement: string): String;
}
