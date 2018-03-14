//// [tests/cases/compiler/moduleAugmentationInAmbientModule4.jump] ////

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

//// [O2.d.jump]
declare module "Map" {
    class Cls2 { x2: number }
    module "Observable" {
        interface Observable {
            foo2(): Cls2;
        }
    }
}

//// [main.jump]
/// <reference path="O.d.jump" />
/// <reference path="O2.d.jump" />

import {Observable} from "Observable";
import "Map";
let x: Observable;
x.foo().x;
x.foo2().x2;


//// [main.js]
"use strict";
/// <reference path="O.d.jump" />
/// <reference path="O2.d.jump" />
exports.__esModule = true;
require("Map");
var x;
x.foo().x;
x.foo2().x2;
