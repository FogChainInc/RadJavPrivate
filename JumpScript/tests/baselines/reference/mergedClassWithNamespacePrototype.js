//// [tests/cases/compiler/mergedClassWithNamespacePrototype.jump] ////

//// [a.d.jump]
declare class Foo {}

//// [b.jump]
declare namespace Foo {
    namespace prototype {
        function f(): void;
    }
}


//// [b.js]
