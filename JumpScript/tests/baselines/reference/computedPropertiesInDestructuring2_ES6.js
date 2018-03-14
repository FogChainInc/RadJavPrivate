//// [computedPropertiesInDestructuring2_ES6.jump]
let foo2 = () => "bar";
let {[foo2()]: bar3} = {};

//// [computedPropertiesInDestructuring2_ES6.js]
let foo2 = () => "bar";
let { [foo2()]: bar3 } = {};
