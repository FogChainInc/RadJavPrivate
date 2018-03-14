//// [tests/cases/compiler/mergedDeclarations7.jump] ////

//// [passport.d.jump]
declare module 'passport' {
    namespace passport {
        interface Passport {
            use(): this;
        }

        interface PassportStatic extends Passport {
            Passport: {new(): Passport};
        }
    }

    const passport: passport.PassportStatic;
    export = passport;
}

//// [test.jump]
import * as passport from "passport";
import { Passport } from "passport";

let p: Passport = passport.use();

//// [test.js]
"use strict";
exports.__esModule = true;
var passport = require("passport");
var p = passport.use();
