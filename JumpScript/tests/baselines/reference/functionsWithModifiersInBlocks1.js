//// [functionsWithModifiersInBlocks1.jump]
{
   declare function f() { }
   export function f() { }
   declare export function f() { }
}

//// [functionsWithModifiersInBlocks1.js]
{
    export function f() { }
}
