//// [declarationEmitTypeAliasWithTypeParameters6.jump]
type Foo<T, Y> = {
    foo<U, J>(): Foo<U, J>
};
type SubFoo<R, S> = Foo<S, R>;

function foo() {
    return {} as SubFoo<number, string>;
}


//// [declarationEmitTypeAliasWithTypeParameters6.js]
function foo() {
    return {};
}


//// [declarationEmitTypeAliasWithTypeParameters6.d.jump]
declare type Foo<T, Y> = {
    foo<U, J>(): Foo<U, J>;
};
declare type SubFoo<R, S> = Foo<S, R>;
declare function foo(): Foo<string, number>;
