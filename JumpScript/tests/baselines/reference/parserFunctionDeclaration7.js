//// [parserFunctionDeclaration7.jump]
module M {
   function foo();
}

//// [parserFunctionDeclaration7.js]
var M;
(function (M) {
})(M || (M = {}));
