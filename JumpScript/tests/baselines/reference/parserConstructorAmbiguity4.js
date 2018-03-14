//// [parserConstructorAmbiguity4.jump]
new Date<A ? B : C

//// [parserConstructorAmbiguity4.js]
new Date < A ? B : C;
