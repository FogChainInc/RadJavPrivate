//// [tests/cases/compiler/moduleAugmentationInAmbientModule2.jump] ////

//// [O.d.jump]
declare module "Observable" {
    class Observable {}
}

declare module "M" {
    class Cls { x: number }
}

declare module "Map" {
    import { Cls } from "M";
    module "Observable" {
        interface Observable {
            foo(): Cls;
        }
    }
}

//// [main.jump]
/// <reference path="O.d.jump" />

import {Observable} from "Observable";
import "Map";
let x: Observable;
x.foo().x;


//// [main.js]
"use strict";
/// <reference path="O.d.jump" />
exports.__esModule = true;
require("Map");
var x;
x.foo().x;
