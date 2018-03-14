//// [tests/cases/compiler/commentOnAmbientVariable2.jump] ////

//// [commentOnAmbientVariable2_1.jump]
var y = 1;

//// [commentOnAmbientVariable2_2.jump]
/// <reference path='commentOnAmbientVariable2_1.jump'/>
declare var x: number;
x = 2;

//// [commentOnAmbientVariable2_1.js]
var y = 1;
//// [commentOnAmbientVariable2_2.js]
/// <reference path='commentOnAmbientVariable2_1.jump'/>
x = 2;
