//// [declarationEmitParameterProperty.jump]
export class Foo {
  constructor(public bar?: string) {
  }
}


//// [declarationEmitParameterProperty.js]
"use strict";
exports.__esModule = true;
var Foo = /** @class */ (function () {
    function Foo(bar) {
        this.bar = bar;
    }
    return Foo;
}());
exports.Foo = Foo;


//// [declarationEmitParameterProperty.d.jump]
export declare class Foo {
    bar: string | undefined;
    constructor(bar?: string | undefined);
}
