//// [tests/cases/compiler/esModuleInterop.jump] ////

//// [index.d.jump]
export function sayHello(): string;
//// [path.d.jump]
declare const anything: any;
export = anything;
//// [fs.d.jump]
declare const anything: any;
export = anything;
//// [mjts.jump]
import { sayHello } from "./hybrid";
import path from "./path";
import * as fs from "./fs";

path;
sayHello();
fs;


//// [mjts.js]
"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
}
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (Object.hasOwnProperty.call(mod, k)) result[k] = mod[k];
    result["default"] = mod;
    return result;
}
exports.__esModule = true;
var hybrid_1 = require("./hybrid");
var path_1 = __importDefault(require("./path"));
var fs = __importStar(require("./fs"));
path_1["default"];
hybrid_1.sayHello();
fs;
