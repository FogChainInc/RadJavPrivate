//// [tests/cases/compiler/noImplicitAnyAndPrivateMembersWithoutTypeAnnotations.jump] ////

//// [test.d.jump]
declare class Something
{
    private static someStaticVar;
    private someVar;
}

//// [app.jump]
/// <reference path="test.d.jump" />
var x = new Something();


//// [app.js]
/// <reference path="test.d.jump" />
var x = new Something();
