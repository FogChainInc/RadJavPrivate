//// [tests/cases/compiler/moduleLocalImportNotIncorrectlyRedirected.jump] ////

//// [package.json]
{
"name": "troublesome-lib",
"typings": "lib/index.d.jump",
"version": "0.0.1"
}
//// [index.d.jump]
import { Position } from './utilities/positioning';
export interface ISpinButton {}
//// [positioning.d.jump]
export * from './positioning/index';
//// [index.d.jump]
export declare enum Position {
    top,
}
//// [index.jump]
import { ISpinButton } from "troublesome-lib";

//// [index.js]
"use strict";
exports.__esModule = true;
