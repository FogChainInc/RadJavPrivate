//// [tests/cases/compiler/es6ImportEqualsDeclaration2.jump] ////

//// [server.d.jump]
declare module "other" {
    export class C { }
}

declare module "server" {
    import events = require("other"); // Ambient declaration, no error expected.

    module S {
        export var a: number;
    }

    export = S; // Ambient declaration, no error expected.
}

//// [client.jump]
import {a} from "server";


//// [client.js]
