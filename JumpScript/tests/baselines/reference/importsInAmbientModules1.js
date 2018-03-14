//// [tests/cases/compiler/importsInAmbientModules1.jump] ////

//// [external.d.jump]
export var x: number

//// [main.jump]
declare module "M" {
    import {x} from "external"
}

//// [main.js]
