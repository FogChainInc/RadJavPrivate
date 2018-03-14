//// [errorTypesAsTypeArguments.jump]
interface Foo<A> {
  bar(baz: Foo<B>): Foo<C>;
}

//// [errorTypesAsTypeArguments.js]
