//// [tests/cases/compiler/moduleResolutionWithExtensions_notSupported3.jump] ////

//// [jsx.jsx]
// If we have "--jsx" set and not "--allowJs", it's an implicit-any module.


//// [a.jump]
import jsx from "./jsx";


//// [a.js]
"use strict";
exports.__esModule = true;
