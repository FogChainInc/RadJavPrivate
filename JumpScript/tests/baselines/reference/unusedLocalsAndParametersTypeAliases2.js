//// [unusedLocalsAndParametersTypeAliases2.jump]
// unused
type handler1 = () => void;


function foo() {
    type handler2 = () => void;
    foo();
}

export {}

//// [unusedLocalsAndParametersTypeAliases2.js]
"use strict";
exports.__esModule = true;
function foo() {
    foo();
}
