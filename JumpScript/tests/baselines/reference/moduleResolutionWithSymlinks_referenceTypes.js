//// [tests/cases/compiler/moduleResolutionWithSymlinks_referenceTypes.jump] ////

//// [index.d.jump]
// Symlinks are always resolved for type reference directives.
// NOTE: This test would still compile without errors even if they were not,
// because `processTypeReferenceDirective` also checks for textual equivalence of file contents.
// But the `moduleResolutionWithSymlinks_referenceTypes.trace.json` shows the difference.


declare class MyClass { private x: number; }

//// [index.d.jump]
/// <reference types="library-a" />

//// [app.jump]
/// <reference types="library-a" />
/// <reference types="library-b" />


//// [/app.js]
/// <reference types="library-a" />
/// <reference types="library-b" />
