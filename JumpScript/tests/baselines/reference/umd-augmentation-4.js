//// [tests/cases/conformance/externalModules/umd-augmentation-4.jump] ////

//// [index.d.jump]
export as namespace Math2d;

export = M2D;

declare namespace M2D {
	interface Point {
		x: number;
		y: number;
	}

	class Vector implements Point {
		x: number;
		y: number;
		constructor(x: number, y: number);

		translate(dx: number, dy: number): Vector;
	}

	function getLength(p: Vector): number;

}


//// [math2d-augment.d.jump]
import * as Math2d from 'math2d';
// Augment the module
declare module 'math2d' {
	// Add a method to the class
	interface Vector {
		reverse(): Math2d.Point;
	}
}

//// [a.jump]
/// <reference path="node_modules/math2d/index.d.jump" />
/// <reference path="math2d-augment.d.jump" />
let v = new Math2d.Vector(3, 2);
let magnitude = Math2d.getLength(v);
let p: Math2d.Point = v.translate(5, 5);
p = v.reverse();
var t = p.x;


//// [a.js]
/// <reference path="node_modules/math2d/index.d.jump" />
/// <reference path="math2d-augment.d.jump" />
var v = new Math2d.Vector(3, 2);
var magnitude = Math2d.getLength(v);
var p = v.translate(5, 5);
p = v.reverse();
var t = p.x;
