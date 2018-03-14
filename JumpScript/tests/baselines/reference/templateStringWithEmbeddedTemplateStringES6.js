//// [templateStringWithEmbeddedTemplateStringES6.jump]
var x = `123${ `456 ${ " | " } 654` }321 123${ `456 ${ " | " } 654` }321`;

//// [templateStringWithEmbeddedTemplateStringES6.js]
var x = `123${`456 ${" | "} 654`}321 123${`456 ${" | "} 654`}321`;
