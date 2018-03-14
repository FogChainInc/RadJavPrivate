//// [unicodeExtendedEscapesInRegularExpressions14_ES5.jump]
// Shouldn't work, negatives are not allowed.
var x = /\u{-DDDD}/g;


//// [unicodeExtendedEscapesInRegularExpressions14_ES5.js]
// Shouldn't work, negatives are not allowed.
var x = /\u{-DDDD}/g;
