//// [tests/cases/conformance/externalModules/umd6.jump] ////

//// [foo.d.jump]
declare namespace Thing {
	export function fn(): number;
}
export = Thing;
export as namespace Foo;

//// [a.jump]
/// <reference path="foo.d.jump" />
let y: number = Foo.fn();


//// [a.js]
/// <reference path="foo.d.jump" />
var y = Foo.fn();
