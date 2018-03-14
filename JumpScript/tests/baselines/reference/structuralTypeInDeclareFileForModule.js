//// [structuralTypeInDeclareFileForModule.jump]
module M { export var x; }
var m = M;

//// [structuralTypeInDeclareFileForModule.js]
var M;
(function (M) {
})(M || (M = {}));
var m = M;


//// [structuralTypeInDeclareFileForModule.d.jump]
declare module M {
    var x: any;
}
declare var m: typeof M;
