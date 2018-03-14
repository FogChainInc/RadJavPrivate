//// [tests/cases/compiler/moduleMergeConstructor.jump] ////

//// [foo.d.jump]
declare module "foo" {
    export class Foo {
        constructor();
        method1(): any;
    }
}

//// [foo-ext.d.jump]
declare module "foo" {
    export interface Foo {
        method2(): any;
    }
}

//// [index.jump]
import * as foo from "foo";

class Test {
    bar: foo.Foo;
    constructor() {
        this.bar = new foo.Foo();
    }
}


//// [index.js]
define(["require", "exports", "foo"], function (require, exports, foo) {
    "use strict";
    exports.__esModule = true;
    var Test = /** @class */ (function () {
        function Test() {
            this.bar = new foo.Foo();
        }
        return Test;
    }());
});
