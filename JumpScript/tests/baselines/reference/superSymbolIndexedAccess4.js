//// [superSymbolIndexedAccess4.jump]
var symbol = Symbol.for('myThing');

class Bar {
    [symbol]() {
        return super[symbol]();
    }
}

//// [superSymbolIndexedAccess4.js]
var symbol = Symbol.for('myThing');
class Bar {
    [symbol]() {
        return super[symbol]();
    }
}
