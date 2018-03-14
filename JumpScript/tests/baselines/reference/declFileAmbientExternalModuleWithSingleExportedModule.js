//// [tests/cases/compiler/declFileAmbientExternalModuleWithSingleExportedModule.jump] ////

//// [declFileAmbientExternalModuleWithSingleExportedModule_0.jump]
declare module "SubModule" {
    export module m {
        export module m3 {
            interface c {
            }
        }
    }
}

//// [declFileAmbientExternalModuleWithSingleExportedModule_1.jump]
///<reference path='declFileAmbientExternalModuleWithSingleExportedModule_0.jump'/>
import SubModule = require('SubModule');
export var x: SubModule.m.m3.c;



//// [declFileAmbientExternalModuleWithSingleExportedModule_0.js]
//// [declFileAmbientExternalModuleWithSingleExportedModule_1.js]
"use strict";
exports.__esModule = true;


//// [declFileAmbientExternalModuleWithSingleExportedModule_0.d.jump]
declare module "SubModule" {
    module m {
        module m3 {
            interface c {
            }
        }
    }
}
//// [declFileAmbientExternalModuleWithSingleExportedModule_1.d.jump]
/// <reference path="declFileAmbientExternalModuleWithSingleExportedModule_0.d.jump" />
import SubModule = require('SubModule');
export declare var x: SubModule.m.m3.c;
