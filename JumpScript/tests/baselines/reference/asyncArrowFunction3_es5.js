//// [asyncArrowFunction3_es5.jump]
function f(await = await) {
}

//// [asyncArrowFunction3_es5.js]
function f(await) {
    if (await === void 0) { await = await; }
}
