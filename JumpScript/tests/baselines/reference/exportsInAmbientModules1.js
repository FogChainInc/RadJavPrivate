//// [tests/cases/compiler/exportsInAmbientModules1.jump] ////

//// [external.d.jump]
export var x: number

//// [main.jump]
declare module "M" {
    export {x} from "external"
}

//// [main.js]
