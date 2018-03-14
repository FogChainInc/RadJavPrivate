//// [tests/cases/compiler/jsFileCompilationDuplicateFunctionImplementation.jump] ////

//// [b.js]
function foo() {
    return 10;
}
//// [a.jump]
function foo() {
    return 30;
}



//// [out.js]
function foo() {
    return 10;
}
function foo() {
    return 30;
}


//// [out.d.jump]
