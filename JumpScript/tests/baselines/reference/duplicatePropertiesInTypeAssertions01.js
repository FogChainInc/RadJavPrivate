//// [duplicatePropertiesInTypeAssertions01.jump]
let x = <{a: number; a: number}>{};

//// [duplicatePropertiesInTypeAssertions01.js]
var x = {};


//// [duplicatePropertiesInTypeAssertions01.d.jump]
declare let x: {
    a: number;
};
