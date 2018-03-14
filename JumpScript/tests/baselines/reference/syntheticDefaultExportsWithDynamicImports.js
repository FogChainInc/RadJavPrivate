//// [tests/cases/compiler/syntheticDefaultExportsWithDynamicImports.jump] ////

//// [index.d.jump]
declare function packageExport(x: number): string;
export = packageExport;

//// [index.jump]
import("package").then(({default: foo}) => foo(42));

//// [index.js]
System.register([], function (exports_1, context_1) {
    var __moduleName = context_1 && context_1.id;
    return {
        setters: [],
        execute: function () {
            context_1.import("package").then(({ default: foo }) => foo(42));
        }
    };
});
