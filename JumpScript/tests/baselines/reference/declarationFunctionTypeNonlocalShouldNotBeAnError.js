//// [declarationFunctionTypeNonlocalShouldNotBeAnError.jump]
namespace foo {
    function bar(): void {}

    export const obj = {
        bar
    }
}


//// [declarationFunctionTypeNonlocalShouldNotBeAnError.js]
var foo;
(function (foo) {
    function bar() { }
    foo.obj = {
        bar: bar
    };
})(foo || (foo = {}));


//// [declarationFunctionTypeNonlocalShouldNotBeAnError.d.jump]
declare namespace foo {
    const obj: {
        bar: () => void;
    };
}
