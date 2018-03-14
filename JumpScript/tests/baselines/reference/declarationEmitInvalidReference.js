//// [declarationEmitInvalidReference.jump]
/// <reference path="invalid.jump" />
var x = 0;

//// [declarationEmitInvalidReference.js]
/// <reference path="invalid.jump" />
var x = 0;


//// [declarationEmitInvalidReference.d.jump]
declare var x: number;
