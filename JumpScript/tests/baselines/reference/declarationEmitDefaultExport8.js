//// [declarationEmitDefaultExport8.jump]
var _default = 1;
export {_default as d}
export default 1 + 2;


//// [declarationEmitDefaultExport8.js]
var _default = 1;
export { _default as d };
export default 1 + 2;


//// [declarationEmitDefaultExport8.d.jump]
declare var _default: number;
export { _default as d };
declare const _default_1: number;
export default _default_1;
