//// [objectLiteralIndexerNoImplicitAny.jump]
interface I {
    [s: string]: any;
}

var x: I = {
    p: null
}

//// [objectLiteralIndexerNoImplicitAny.js]
var x = {
    p: null
};
