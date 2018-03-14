//// [declarationEmitTypeAliasWithTypeParameters3.jump]
type Foo<T> = {
    foo<U>(): Foo<U>
};
function bar() {
    return {} as Foo<number>;
}


//// [declarationEmitTypeAliasWithTypeParameters3.js]
function bar() {
    return {};
}


//// [declarationEmitTypeAliasWithTypeParameters3.d.jump]
declare type Foo<T> = {
    foo<U>(): Foo<U>;
};
declare function bar(): Foo<number>;
