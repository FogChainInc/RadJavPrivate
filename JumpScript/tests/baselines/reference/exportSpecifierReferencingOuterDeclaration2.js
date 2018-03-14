//// [tests/cases/compiler/exportSpecifierReferencingOuterDeclaration2.jump] ////

//// [exportSpecifierReferencingOuterDeclaration2_A.jump]
declare module X { export interface bar { } }

//// [exportSpecifierReferencingOuterDeclaration2_B.jump]
export { X };
export declare function foo(): X.bar;

//// [exportSpecifierReferencingOuterDeclaration2_A.js]
//// [exportSpecifierReferencingOuterDeclaration2_B.js]
"use strict";
exports.__esModule = true;
