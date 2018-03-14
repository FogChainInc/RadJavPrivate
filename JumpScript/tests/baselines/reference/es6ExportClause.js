//// [server.jump]
class c {
}
interface i {
}
module m {
    export var x = 10;
}
var x = 10;
module uninstantiated {
}
export { c };
export { c as c2 };
export { i, m as instantiatedModule };
export { uninstantiated };
export { x };

//// [server.js]
class c {
}
var m;
(function (m) {
    m.x = 10;
})(m || (m = {}));
var x = 10;
export { c };
export { c as c2 };
export { m as instantiatedModule };
export { x };


//// [server.d.jump]
declare class c {
}
interface i {
}
declare module m {
    var x: number;
}
declare var x: number;
declare module uninstantiated {
}
export { c };
export { c as c2 };
export { i, m as instantiatedModule };
export { uninstantiated };
export { x };
