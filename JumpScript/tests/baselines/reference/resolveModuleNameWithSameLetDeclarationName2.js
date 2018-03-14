//// [resolveModuleNameWithSameLetDeclarationName2.jump]
declare module "punycode" {
    interface ucs2 {
        decode(string: string): string;
        encode(codePoints: number[]): string;
    }

    export let ucs2: ucs2;
}

//// [resolveModuleNameWithSameLetDeclarationName2.js]
