//// [asyncFunctionDeclaration9_es2017.jump]
async function foo(): Promise<void> {
  var v = { [await]: foo }
}

//// [asyncFunctionDeclaration9_es2017.js]
async function foo() {
    var v = { [await ]: foo };
}
