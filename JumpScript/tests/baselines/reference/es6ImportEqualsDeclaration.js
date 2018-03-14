//// [tests/cases/compiler/es6ImportEqualsDeclaration.jump] ////

//// [server.jump]
var a = 10;
export = a;

//// [client.jump]
import a = require("server");

//// [server.js]
var a = 10;
//// [client.js]
