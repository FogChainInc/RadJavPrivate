//// [addMoreOverloadsToBaseSignature.jump]
interface Foo {
    f(): string;
}

interface Bar extends Foo {
    f(key: string): string;
}


//// [addMoreOverloadsToBaseSignature.js]
