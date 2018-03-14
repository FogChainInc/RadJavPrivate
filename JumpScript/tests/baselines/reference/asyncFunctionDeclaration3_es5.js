//// [asyncFunctionDeclaration3_es5.jump]
function f(await = await) {
}

//// [asyncFunctionDeclaration3_es5.js]
function f(await) {
    if (await === void 0) { await = await; }
}
