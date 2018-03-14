//// [tests/cases/compiler/funduleSplitAcrossFiles.jump] ////

//// [funduleSplitAcrossFiles_function.jump]
function D() { }

//// [funduleSplitAcrossFiles_module.jump]
module D {
    export var y = "hi";
}
D.y;

//// [funduleSplitAcrossFiles_function.js]
function D() { }
//// [funduleSplitAcrossFiles_module.js]
var D;
(function (D) {
    D.y = "hi";
})(D || (D = {}));
D.y;
