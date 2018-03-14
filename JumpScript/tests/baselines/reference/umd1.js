//// [tests/cases/conformance/externalModules/umd1.jump] ////

//// [foo.d.jump]
export var x: number;
export function fn(): void;
export interface Thing { n: typeof x }
export as namespace Foo;

//// [a.jump]
/// <reference path="foo.d.jump" />
Foo.fn();
let x: Foo.Thing;
let y: number = x.n;


//// [a.js]
/// <reference path="foo.d.jump" />
Foo.fn();
var x;
var y = x.n;
