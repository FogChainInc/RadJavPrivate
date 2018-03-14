//// [tests/cases/compiler/constEnumNamespaceReferenceCausesNoImport.jump] ////

//// [foo.jump]
export const enum ConstFooEnum {
    Some,
    Values,
    Here
};
export function fooFunc(): void { /* removed */ }
//// [index.jump]
import * as Foo from "./foo";

function check(x: Foo.ConstFooEnum): void {
  switch (x) {
    case Foo.ConstFooEnum.Some:
      break;
  }
}

//// [foo.js]
"use strict";
exports.__esModule = true;
;
function fooFunc() { }
exports.fooFunc = fooFunc;
//// [index.js]
"use strict";
exports.__esModule = true;
function check(x) {
    switch (x) {
        case 0 /* Some */:
            break;
    }
}
