//// [FunctionDeclaration7.jump]
module M {
   function foo();
}

//// [FunctionDeclaration7.js]
var M;
(function (M) {
})(M || (M = {}));
