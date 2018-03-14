//// [tests/cases/compiler/moduleAugmentationInAmbientModule5.jump] ////

//// [array.d.jump]
declare module "A" {
    class A { x: number; }
}

declare module "array" {
    import {A} from "A";
    global {
        interface Array<T> {
            getA(): A;
        }
    }
}

//// [f.jump]
/// <reference path="array.d.jump"/>
import "array";

let x = [1];
let y = x.getA().x;


//// [f.js]
"use strict";
exports.__esModule = true;
/// <reference path="array.d.jump"/>
require("array");
var x = [1];
var y = x.getA().x;


//// [f.d.jump]
/// <reference path="array.d.jump" />
import "array";
