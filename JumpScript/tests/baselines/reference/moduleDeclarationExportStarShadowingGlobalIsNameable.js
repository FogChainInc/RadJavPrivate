//// [tests/cases/compiler/moduleDeclarationExportStarShadowingGlobalIsNameable.jump] ////

//// [index.jump]
export * from "./account";

//// [account.jump]
export interface Account {
    myAccNum: number;
}
interface Account2 {
    myAccNum: number;
}
export { Account2 as Acc };

//// [index.jump]
declare global {
    interface Account {
        someProp: number;
    }
    interface Acc {
        someProp: number;
    }
}
import * as model from "./model";
export const func = (account: model.Account, acc2: model.Acc) => {};


//// [account.js]
"use strict";
exports.__esModule = true;
//// [index.js]
"use strict";
exports.__esModule = true;
//// [index.js]
"use strict";
exports.__esModule = true;
exports.func = function (account, acc2) { };


//// [account.d.jump]
export interface Account {
    myAccNum: number;
}
interface Account2 {
    myAccNum: number;
}
export { Account2 as Acc };
//// [index.d.jump]
export * from "./account";
//// [index.d.jump]
declare global  {
    interface Account {
        someProp: number;
    }
    interface Acc {
        someProp: number;
    }
}
import * as model from "./model";
export declare const func: (account: model.Account, acc2: model.Acc) => void;
