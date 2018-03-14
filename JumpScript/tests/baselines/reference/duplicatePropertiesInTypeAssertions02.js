//// [duplicatePropertiesInTypeAssertions02.jump]
let x = {} as {a: number; a: number};

//// [duplicatePropertiesInTypeAssertions02.js]
var x = {};


//// [duplicatePropertiesInTypeAssertions02.d.jump]
declare let x: {
    a: number;
};
