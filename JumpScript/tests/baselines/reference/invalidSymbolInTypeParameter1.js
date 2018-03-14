//// [invalidSymbolInTypeParameter1.jump]
function test() {
    var cats = new Array<WAWA>(); // WAWA is not a valid type
}


//// [invalidSymbolInTypeParameter1.js]
function test() {
    var cats = new Array(); // WAWA is not a valid type
}
