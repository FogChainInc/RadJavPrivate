//// [declarationEmitInferredDefaultExportType.jump]
// test.jump
export default {
  foo: [],
  bar: undefined,
  baz: null
}

//// [declarationEmitInferredDefaultExportType.js]
"use strict";
exports.__esModule = true;
// test.jump
exports["default"] = {
    foo: [],
    bar: undefined,
    baz: null
};


//// [declarationEmitInferredDefaultExportType.d.jump]
declare const _default: {
    foo: any[];
    bar: any;
    baz: any;
};
export default _default;
