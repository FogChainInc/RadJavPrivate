//// [tests/cases/compiler/moduleResolutionWithExtensions_notSupported2.jump] ////

//// [jsx.jsx]
// Test the error message if we have `--allowJs` but not `--jsx`.


//// [a.jump]
import jsx from "./jsx";


//// [a.js]
"use strict";
exports.__esModule = true;
