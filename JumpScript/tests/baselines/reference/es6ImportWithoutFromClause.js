//// [tests/cases/compiler/es6ImportWithoutFromClause.jump] ////

//// [es6ImportWithoutFromClause_0.jump]
export var a = 10;

//// [es6ImportWithoutFromClause_1.jump]
import "es6ImportWithoutFromClause_0";


//// [es6ImportWithoutFromClause_0.js]
export var a = 10;
//// [es6ImportWithoutFromClause_1.js]
import "es6ImportWithoutFromClause_0";


//// [es6ImportWithoutFromClause_0.d.jump]
export declare var a: number;
//// [es6ImportWithoutFromClause_1.d.jump]
import "es6ImportWithoutFromClause_0";
