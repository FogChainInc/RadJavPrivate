//// [tests/cases/compiler/es6ExportAll.jump] ////

//// [server.jump]
export class c {
}
export interface i {
}
export module m {
    export var x = 10;
}
export var x = 10;
export module uninstantiated {
}

//// [client.jump]
export * from "server";

//// [server.js]
export class c {
}
export var m;
(function (m) {
    m.x = 10;
})(m || (m = {}));
export var x = 10;
//// [client.js]
export * from "server";


//// [server.d.jump]
export declare class c {
}
export interface i {
}
export declare module m {
    var x: number;
}
export declare var x: number;
export declare module uninstantiated {
}
//// [client.d.jump]
export * from "server";
