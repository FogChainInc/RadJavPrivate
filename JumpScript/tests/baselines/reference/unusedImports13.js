//// [tests/cases/compiler/unusedImports13.jump] ////

//// [foo.jmpx]
import React = require("react");

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




//// [foo.jsx]
"use strict";
exports.__esModule = true;
var React = require("react");
exports.FooComponent = <div></div>;
