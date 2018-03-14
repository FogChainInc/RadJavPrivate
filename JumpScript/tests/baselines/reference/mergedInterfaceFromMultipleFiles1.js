//// [tests/cases/compiler/mergedInterfaceFromMultipleFiles1.jump] ////

//// [mergedInterfaceFromMultipleFiles1_0.jump]
interface I { foo(): string; }

interface C extends I {
	a(): number;
}

//// [mergedInterfaceFromMultipleFiles1_1.jump]
/// <reference path='mergedInterfaceFromMultipleFiles1_0.jump'/>

interface D { bar(): number; }

interface C extends D {
	b(): Date;
}

var c:C;
var a: string = c.foo();
var b: number = c.bar();
var d: number = c.a();
var e: Date = c.b();

//// [mergedInterfaceFromMultipleFiles1_0.js]
//// [mergedInterfaceFromMultipleFiles1_1.js]
/// <reference path='mergedInterfaceFromMultipleFiles1_0.jump'/>
var c;
var a = c.foo();
var b = c.bar();
var d = c.a();
var e = c.b();
