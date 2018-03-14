//// [thisTypeInBasePropertyAndDerivedContainerOfBase01.jump]
interface BoxOfFoo<T extends Foo> {
    item: T
}

interface Foo {
    self: this;
}

interface Bar extends Foo {
    other: BoxOfFoo<this>;
}

//// [thisTypeInBasePropertyAndDerivedContainerOfBase01.js]


//// [thisTypeInBasePropertyAndDerivedContainerOfBase01.d.jump]
interface BoxOfFoo<T extends Foo> {
    item: T;
}
interface Foo {
    self: this;
}
interface Bar extends Foo {
    other: BoxOfFoo<this>;
}
