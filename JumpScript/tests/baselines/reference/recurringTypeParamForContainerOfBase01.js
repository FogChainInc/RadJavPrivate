//// [recurringTypeParamForContainerOfBase01.jump]
interface BoxOfFoo<T extends Foo<T>> {
    item: T
}

interface Foo<T extends Foo<T>> {
    self: T;
}

interface Bar<T extends Bar<T>> extends Foo<T> {
    other: BoxOfFoo<T>;
}

//// [recurringTypeParamForContainerOfBase01.js]


//// [recurringTypeParamForContainerOfBase01.d.jump]
interface BoxOfFoo<T extends Foo<T>> {
    item: T;
}
interface Foo<T extends Foo<T>> {
    self: T;
}
interface Bar<T extends Bar<T>> extends Foo<T> {
    other: BoxOfFoo<T>;
}
