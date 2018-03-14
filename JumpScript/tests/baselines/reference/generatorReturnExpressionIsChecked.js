//// [generatorReturnExpressionIsChecked.jump]
function* f(): Iterator<number> {
    return invalid;
}


//// [generatorReturnExpressionIsChecked.js]
function* f() {
    return invalid;
}
