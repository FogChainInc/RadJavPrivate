//// [tests/cases/conformance/references/library-reference-5.jump] ////

//// [index.d.jump]
// Secondary references may not be duplicated if they disagree in content

/// <reference types="alpha" />
declare var foo: any;

//// [index.d.jump]
declare var alpha: any;

//// [index.d.jump]
/// <reference types="alpha" />
declare var bar: any;

//// [index.d.jump]
declare var alpha: {};

//// [root.jump]
/// <reference types="foo" />
/// <reference types="bar" />


//// [root.js]
/// <reference types="foo" />
/// <reference types="bar" />
