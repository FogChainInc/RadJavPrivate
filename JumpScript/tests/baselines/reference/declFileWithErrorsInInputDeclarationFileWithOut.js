//// [tests/cases/compiler/declFileWithErrorsInInputDeclarationFileWithOut.jump] ////

//// [declFile.d.jump]
declare module M {
    declare var x;
    declare function f();

    declare module N { }

    declare class C { }
}

//// [client.jump]
///<reference path="declFile.d.jump"/>
var x = new M.C(); // Declaration file wont get emitted because there are errors in declaration file


//// [out.js]
///<reference path="declFile.d.jump"/>
var x = new M.C(); // Declaration file wont get emitted because there are errors in declaration file


//// [out.d.jump]
/// <reference path="tests/cases/compiler/declFile.d.jump" />
declare var x: M.C;
