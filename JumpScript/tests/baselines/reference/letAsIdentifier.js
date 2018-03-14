//// [letAsIdentifier.jump]
var let = 10;
var a = 10;
let = 30;
let
a;

//// [letAsIdentifier.js]
var let = 10;
var a = 10;
let = 30;
var a;


//// [letAsIdentifier.d.jump]
declare var let: number;
declare var a: number;
declare let a: any;
