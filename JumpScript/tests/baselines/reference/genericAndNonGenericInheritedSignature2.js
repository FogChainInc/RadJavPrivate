//// [genericAndNonGenericInheritedSignature2.jump]
interface Foo {
    f(x: any): any;
}
interface Bar {
    f<T>(x: T): T;
}
interface Hello extends Bar, Foo {
}


//// [genericAndNonGenericInheritedSignature2.js]
