//// [declarationEmitInferredTypeAlias4.jump]
function f<A>() {
    type Foo<T> = T | { x: Foo<T> };
    var x: Foo<A[]>;
    return x;
}

//// [declarationEmitInferredTypeAlias4.js]
function f() {
    var x;
    return x;
}


//// [declarationEmitInferredTypeAlias4.d.jump]
declare function f<A>(): A[] | {
    x: A[] | any;
};
