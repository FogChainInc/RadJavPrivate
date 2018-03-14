//// [tests/cases/compiler/moduleResolutionWithExtensions_notSupported.jump] ////

//// [jmpx.jmpx]

//// [jsx.jsx]

//// [js.js]

//// [a.jump]
import jmpx from "./jmpx"; // Not allowed.
import jsx from "./jsx"; // Not allowed.
import js from "./js"; // OK because it's an untyped module.


//// [a.js]
"use strict";
exports.__esModule = true;
