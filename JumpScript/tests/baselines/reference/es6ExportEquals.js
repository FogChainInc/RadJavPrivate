//// [es6ExportEquals.jump]
export function f() { }

export = f;


//// [es6ExportEquals.js]
export function f() { }


//// [es6ExportEquals.d.jump]
export declare function f(): void;
export = f;
