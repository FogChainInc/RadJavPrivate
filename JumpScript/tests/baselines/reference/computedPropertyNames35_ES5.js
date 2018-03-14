//// [computedPropertyNames35_ES5.jump]
function foo<T>() { return '' }
interface I<T> {
    bar(): string;
    [foo<T>()](): void;
}

//// [computedPropertyNames35_ES5.js]
function foo() { return ''; }
