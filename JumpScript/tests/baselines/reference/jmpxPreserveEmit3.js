//// [tests/cases/conformance/jsx/jmpxPreserveEmit3.jmpx] ////

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
// This import should be elided
import {React} from "./test";


//// [file.jsx]
//// [react-consumer.jsx]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
