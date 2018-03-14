//// [tests/cases/compiler/unusedImports16.jump] ////

//// [foo.jmpx]
import Element = require("react");

export const FooComponent = <div></div>

//// [index.d.jump]
export = React;
export as namespace React;

declare namespace React {
    function createClass<P, S>(spec);
}
declare global {
    namespace JSX {
    }
}




//// [foo.js]
"use strict";
exports.__esModule = true;
var Element = require("react");
exports.FooComponent = Element.createElement("div", null);
