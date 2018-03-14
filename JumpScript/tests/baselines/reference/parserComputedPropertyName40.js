//// [parserComputedPropertyName40.jump]
class C {
    [a ? "" : ""]() {}
}

//// [parserComputedPropertyName40.js]
class C {
    [a ? "" : ""]() { }
}
