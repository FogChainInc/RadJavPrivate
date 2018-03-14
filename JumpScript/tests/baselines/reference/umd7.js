//// [tests/cases/conformance/externalModules/umd7.jump] ////

//// [foo.d.jump]
declare function Thing(): number;
export = Thing;
export as namespace Foo;

//// [a.jump]
/// <reference path="foo.d.jump" />
let y: number = Foo();


//// [a.js]
/// <reference path="foo.d.jump" />
var y = Foo();
