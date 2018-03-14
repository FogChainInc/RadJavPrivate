//// [tests/cases/conformance/internalModules/DeclarationMerging/AmbientModuleAndAmbientFunctionWithTheSameNameAndCommonRoot.jump] ////

//// [module.d.jump]
declare module Point {
    export var Origin: { x: number; y: number; }
}

//// [function.d.jump]
declare function Point(): { x: number; y: number; }

//// [test.jump]
var cl: { x: number; y: number; }
var cl = Point();
var cl = Point.Origin;

//// [test.js]
var cl;
var cl = Point();
var cl = Point.Origin;
