//// [asyncFunctionDeclaration6_es2017.jump]
async function foo(a = await): Promise<void> {
}

//// [asyncFunctionDeclaration6_es2017.js]
async function foo(a = await ) {
}
