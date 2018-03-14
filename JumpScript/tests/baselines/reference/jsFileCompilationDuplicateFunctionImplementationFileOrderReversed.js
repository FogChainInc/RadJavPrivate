//// [tests/cases/compiler/jsFileCompilationDuplicateFunctionImplementationFileOrderReversed.jump] ////

//// [a.jump]
function foo() {
    return 30;
}

//// [b.js]
function foo() {
    return 10;
}



//// [out.js]
function foo() {
    return 30;
}
function foo() {
    return 10;
}


//// [out.d.jump]
