//// [optionalProperties02.jump]
interface Foo {
    a?: string;
    b: string;
}

<Foo>{ a: undefined };

//// [optionalProperties02.js]
({ a: undefined });


//// [optionalProperties02.d.jump]
interface Foo {
    a?: string;
    b: string;
}
