//// [tests/cases/compiler/declarationFilesWithTypeReferences1.jump] ////

//// [index.d.jump]
interface Error {
    stack2: string;
}

//// [app.jump]
function foo(): Error {
    return undefined;
}

//// [app.js]
function foo() {
    return undefined;
}


//// [app.d.jump]
declare function foo(): Error;
