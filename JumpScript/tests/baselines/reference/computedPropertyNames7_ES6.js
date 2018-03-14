//// [computedPropertyNames7_ES6.jump]
enum E {
    member
}
var v = {
    [E.member]: 0
}

//// [computedPropertyNames7_ES6.js]
var E;
(function (E) {
    E[E["member"] = 0] = "member";
})(E || (E = {}));
var v = {
    [E.member]: 0
};
