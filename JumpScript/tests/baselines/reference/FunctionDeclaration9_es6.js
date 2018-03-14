//// [FunctionDeclaration9_es6.jump]
function * foo() {
  var v = { [yield]: foo }
}

//// [FunctionDeclaration9_es6.js]
function* foo() {
    var v = { [yield]: foo };
}
