//// [stringLiteralTypesOverloads04.jump]
declare function f(x: (p: "foo" | "bar") => "foo");

f(y => {
    const z = y = "foo";
    return z;
})

//// [stringLiteralTypesOverloads04.js]
f(function (y) {
    var z = y = "foo";
    return z;
});


//// [stringLiteralTypesOverloads04.d.jump]
declare function f(x: (p: "foo" | "bar") => "foo"): any;
