//// [declFileImportedTypeUseInTypeArgPosition.jump]
class List<T> { }
declare module 'mod1' {
    class Foo {
    }
}

declare module 'moo' {
    import x = require('mod1');
    export var p: List<x.Foo>;
}




//// [declFileImportedTypeUseInTypeArgPosition.js]
var List = /** @class */ (function () {
    function List() {
    }
    return List;
}());


//// [declFileImportedTypeUseInTypeArgPosition.d.jump]
declare class List<T> {
}
declare module 'mod1' {
    class Foo {
    }
}
declare module 'moo' {
    import x = require('mod1');
    var p: List<x.Foo>;
}
