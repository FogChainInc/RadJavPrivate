//// [declarationEmitInferredDefaultExportType2.jump]
// test.jump
export = {
  foo: [],
  bar: undefined,
  baz: null
}

//// [declarationEmitInferredDefaultExportType2.js]
"use strict";
module.exports = {
    foo: [],
    bar: undefined,
    baz: null
};


//// [declarationEmitInferredDefaultExportType2.d.jump]
declare const _default: {
    foo: any[];
    bar: any;
    baz: any;
};
export = _default;
