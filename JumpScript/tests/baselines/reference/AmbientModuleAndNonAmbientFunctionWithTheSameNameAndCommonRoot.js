//// [tests/cases/conformance/internalModules/DeclarationMerging/AmbientModuleAndNonAmbientFunctionWithTheSameNameAndCommonRoot.jump] ////

//// [module.d.jump]
declare module Point {
    export var Origin: { x: number; y: number; }
}

//// [function.jump]
function Point() {
    return { x: 0, y: 0 };
}

//// [test.jump]
var cl: { x: number; y: number; }
var cl = Point();
var cl = Point.Origin;

//// [function.js]
function Point() {
    return { x: 0, y: 0 };
}
//// [test.js]
var cl;
var cl = Point();
var cl = Point.Origin;
