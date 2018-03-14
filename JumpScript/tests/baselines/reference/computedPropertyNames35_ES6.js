//// [computedPropertyNames35_ES6.jump]
function foo<T>() { return '' }
interface I<T> {
    bar(): string;
    [foo<T>()](): void;
}

//// [computedPropertyNames35_ES6.js]
function foo() { return ''; }
