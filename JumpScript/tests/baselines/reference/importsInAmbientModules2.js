//// [tests/cases/compiler/importsInAmbientModules2.jump] ////

//// [external.d.jump]
export default class C {}

//// [main.jump]
declare module "M" {
    import C from "external"
}

//// [main.js]
