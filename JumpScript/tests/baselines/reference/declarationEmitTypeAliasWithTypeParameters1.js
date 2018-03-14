//// [declarationEmitTypeAliasWithTypeParameters1.jump]
export type Bar<X, Y> = () => [X, Y];
export type Foo<Y> = Bar<any, Y>;
export const y = (x: Foo<string>) => 1

//// [declarationEmitTypeAliasWithTypeParameters1.js]
"use strict";
exports.__esModule = true;
exports.y = function (x) { return 1; };


//// [declarationEmitTypeAliasWithTypeParameters1.d.jump]
export declare type Bar<X, Y> = () => [X, Y];
export declare type Foo<Y> = Bar<any, Y>;
export declare const y: (x: Bar<any, string>) => number;
