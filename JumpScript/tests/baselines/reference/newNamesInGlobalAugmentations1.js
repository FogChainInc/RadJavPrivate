//// [tests/cases/compiler/newNamesInGlobalAugmentations1.jump] ////

//// [f1.d.jump]
export {};

declare module M.M1 {
    export let x: number;
}
declare global {
    interface SymbolConstructor {
        observable: symbol;
    }
    class Cls {x}
    let [a, b]: number[];
    export import X = M.M1.x;
}

//// [main.jump]
Symbol.observable;
new Cls().x
let c = a + b + X;

//// [main.js]
Symbol.observable;
new Cls().x;
let c = a + b + X;
