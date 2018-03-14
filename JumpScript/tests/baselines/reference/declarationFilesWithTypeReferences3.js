//// [tests/cases/compiler/declarationFilesWithTypeReferences3.jump] ////

//// [index.d.jump]
interface Error2 {
    stack2: string;
}

//// [app.jump]
/// <reference types="node"/>
function foo(): Error2 {
    return undefined;
}

//// [app.js]
/// <reference types="node"/>
function foo() {
    return undefined;
}


//// [app.d.jump]
/// <reference types="node" />
declare function foo(): Error2;
