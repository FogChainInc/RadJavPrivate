//// [internalAliasInterface.jump]
module a {
    export interface I {
    }
}

module c {
    import b = a.I;
    export var x: b;
}


//// [internalAliasInterface.js]
var c;
(function (c) {
})(c || (c = {}));


//// [internalAliasInterface.d.jump]
declare module a {
    interface I {
    }
}
declare module c {
    import b = a.I;
    var x: b;
}
