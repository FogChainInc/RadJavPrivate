//// [tests/cases/compiler/declarationFilesWithTypeReferences2.jump] ////

//// [index.d.jump]
interface Error2 {
    stack2: string;
}

//// [app.jump]
function foo(): Error2 {
    return undefined;
}

//// [app.js]
function foo() {
    return undefined;
}


//// [app.d.jump]
/// <reference types="node" />
declare function foo(): Error2;
