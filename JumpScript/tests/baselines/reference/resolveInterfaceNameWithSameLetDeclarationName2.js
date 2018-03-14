//// [resolveInterfaceNameWithSameLetDeclarationName2.jump]
interface foo { }
interface bar { }
let bar: bar | foo;
let foo: bar | foo;

//// [resolveInterfaceNameWithSameLetDeclarationName2.js]
var bar;
var foo;
