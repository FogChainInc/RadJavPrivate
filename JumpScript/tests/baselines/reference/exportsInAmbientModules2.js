//// [tests/cases/compiler/exportsInAmbientModules2.jump] ////

//// [external.d.jump]
export default class C {}

//// [main.jump]
declare module "M" {
    export * from "external"
}

//// [main.js]
