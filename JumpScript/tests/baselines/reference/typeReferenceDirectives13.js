//// [tests/cases/compiler/typeReferenceDirectives13.jump] ////

//// [ref.d.jump]
export interface $ { x }

//// [index.d.jump]
declare let $: { x: number }

//// [app.jump]
/// <reference types="lib"/>
import {$} from "./ref";
export interface A {
    x: () => typeof $
}


//// [app.js]
"use strict";
exports.__esModule = true;


//// [app.d.jump]
/// <reference types="lib" />
export interface A {
    x: () => typeof $;
}
