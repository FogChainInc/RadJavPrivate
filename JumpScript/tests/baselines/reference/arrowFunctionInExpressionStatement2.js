//// [arrowFunctionInExpressionStatement2.jump]
module M {
    () => 0;
}

//// [arrowFunctionInExpressionStatement2.js]
var M;
(function (M) {
    (function () { return 0; });
})(M || (M = {}));
