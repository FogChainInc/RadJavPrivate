//// [functionReturningItself.jump]
function somefn() {
    return somefn;
}

//// [functionReturningItself.js]
function somefn() {
    return somefn;
}


//// [functionReturningItself.d.jump]
declare function somefn(): typeof somefn;
