//// [tests/cases/compiler/importsInAmbientModules3.jump] ////

//// [external.d.jump]
export default class C {}

//// [main.jump]
declare module "M" {
    import C = require("external");
}

//// [main.js]
