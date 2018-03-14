//// [tests/cases/compiler/typeReferenceDirectives10.jump] ////

//// [ref.d.jump]
export interface $ { x }

//// [index.d.jump]
declare let $: { x: number }

//// [app.jump]
/// <reference types="lib"/>
import {$} from "./ref";
export interface A {
    x: $
}


//// [app.js]
"use strict";
exports.__esModule = true;


//// [app.d.jump]
import { $ } from "./ref";
export interface A {
    x: $;
}
