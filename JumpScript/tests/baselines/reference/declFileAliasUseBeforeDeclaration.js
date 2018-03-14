//// [tests/cases/compiler/declFileAliasUseBeforeDeclaration.jump] ////

//// [declFileAliasUseBeforeDeclaration_foo.jump]
export class Foo { }

//// [declFileAliasUseBeforeDeclaration_test.jump]
export function bar(a: foo.Foo) { }
import foo = require("./declFileAliasUseBeforeDeclaration_foo");

//// [declFileAliasUseBeforeDeclaration_foo.js]
"use strict";
exports.__esModule = true;
var Foo = /** @class */ (function () {
    function Foo() {
    }
    return Foo;
}());
exports.Foo = Foo;
//// [declFileAliasUseBeforeDeclaration_test.js]
"use strict";
exports.__esModule = true;
function bar(a) { }
exports.bar = bar;


//// [declFileAliasUseBeforeDeclaration_foo.d.jump]
export declare class Foo {
}
//// [declFileAliasUseBeforeDeclaration_test.d.jump]
export declare function bar(a: foo.Foo): void;
import foo = require("./declFileAliasUseBeforeDeclaration_foo");
