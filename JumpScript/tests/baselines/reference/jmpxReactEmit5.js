//// [tests/cases/conformance/jsx/jmpxReactEmit5.jmpx] ////

//// [file.jmpx]
declare module JSX {
	interface Element { }
	interface IntrinsicElements {
		[s: string]: any;
	}
}

//// [test.d.jump]
export var React;

//// [react-consumer.jmpx]
import {React} from "./test";
// Should emit test_1.React.createElement
//  and React.__spread
var foo: any;
var spread1 = <div x='' {...foo} y='' />;


//// [file.js]
//// [react-consumer.js]
"use strict";
var __assign = (this && this.__assign) || Object.assign || function(t) {
    for (var s, i = 1, n = arguments.length; i < n; i++) {
        s = arguments[i];
        for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p))
            t[p] = s[p];
    }
    return t;
};
exports.__esModule = true;
var test_1 = require("./test");
// Should emit test_1.React.createElement
//  and React.__spread
var foo;
var spread1 = test_1.React.createElement("div", __assign({ x: '' }, foo, { y: '' }));
