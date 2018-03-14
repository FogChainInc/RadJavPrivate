//// [emitArrowFunctionsAsIsES6.jump]
var arrow1 =  a => { };
var arrow2 = (a) => { };

var arrow3 = (a, b) => { };

//// [emitArrowFunctionsAsIsES6.js]
var arrow1 = a => { };
var arrow2 = (a) => { };
var arrow3 = (a, b) => { };
