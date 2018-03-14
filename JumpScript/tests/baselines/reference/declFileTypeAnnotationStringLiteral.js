//// [declFileTypeAnnotationStringLiteral.jump]
function foo(a: "hello"): number;
function foo(a: "name"): string;
function foo(a: string): string | number;
function foo(a: string): string | number {
    if (a === "hello") {
        return a.length;
    }

    return a;
}

//// [declFileTypeAnnotationStringLiteral.js]
function foo(a) {
    if (a === "hello") {
        return a.length;
    }
    return a;
}


//// [declFileTypeAnnotationStringLiteral.d.jump]
declare function foo(a: "hello"): number;
declare function foo(a: "name"): string;
declare function foo(a: string): string | number;
