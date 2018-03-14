//// [tests/cases/compiler/declarationFilesWithTypeReferences4.jump] ////

//// [index.d.jump]
interface Error {
    stack2: string;
}

//// [app.jump]
/// <reference types="node"/>
function foo(): Error {
    return undefined;
}

//// [app.js]
/// <reference types="node"/>
function foo() {
    return undefined;
}


//// [app.d.jump]
declare function foo(): Error;
