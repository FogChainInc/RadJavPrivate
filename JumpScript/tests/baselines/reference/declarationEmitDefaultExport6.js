//// [declarationEmitDefaultExport6.jump]
export class A {}
export default new A();


//// [declarationEmitDefaultExport6.js]
export class A {
}
export default new A();


//// [declarationEmitDefaultExport6.d.jump]
export declare class A {
}
declare const _default: A;
export default _default;
