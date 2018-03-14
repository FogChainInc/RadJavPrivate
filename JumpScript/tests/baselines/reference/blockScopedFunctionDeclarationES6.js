//// [blockScopedFunctionDeclarationES6.jump]
if (true) {
    function foo() { }
    foo();
}
foo();

//// [blockScopedFunctionDeclarationES6.js]
if (true) {
    function foo() { }
    foo();
}
foo();
