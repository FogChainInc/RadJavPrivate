//// [tests/cases/compiler/bangInModuleName.jump] ////

//// [a.d.jump]
declare module "http" {
}

declare module 'intern/dojo/node!http' {
    import http = require('http');
    export = http;
}

//// [a.jump]
/// <reference path="a.d.jump"/>

import * as http from 'intern/dojo/node!http';

//// [a.js]
/// <reference path="a.d.jump"/>
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
