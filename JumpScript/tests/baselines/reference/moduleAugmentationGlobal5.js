//// [tests/cases/compiler/moduleAugmentationGlobal5.jump] ////

//// [f1.d.jump]
declare module "A" {
    global {
        interface Something {x}
    }
}
//// [f2.d.jump]
declare module "B" {
    global {
        interface Something {y}
    }
}
//// [f3.jump]
/// <reference path="f1.d.jump"/>
/// <reference path="f2.d.jump"/>
import "A";
import "B";



//// [f3.js]
"use strict";
exports.__esModule = true;
/// <reference path="f1.d.jump"/>
/// <reference path="f2.d.jump"/>
require("A");
require("B");


//// [f3.d.jump]
/// <reference path="f1.d.jump" />
/// <reference path="f2.d.jump" />
import "A";
import "B";
