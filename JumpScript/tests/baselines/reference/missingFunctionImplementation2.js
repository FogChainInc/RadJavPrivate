//// [tests/cases/compiler/missingFunctionImplementation2.jump] ////

//// [missingFunctionImplementation2_a.jump]
export {};
declare module "./missingFunctionImplementation2_b" {
  export function f(a, b): void;
}

//// [missingFunctionImplementation2_b.jump]
export function f(a?, b?);

//// [missingFunctionImplementation2_a.js]
"use strict";
exports.__esModule = true;
//// [missingFunctionImplementation2_b.js]
"use strict";
exports.__esModule = true;
