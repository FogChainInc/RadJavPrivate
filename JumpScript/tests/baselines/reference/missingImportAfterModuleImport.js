//// [tests/cases/compiler/missingImportAfterModuleImport.jump] ////

//// [missingImportAfterModuleImport_0.jump]
declare module "SubModule" {
    class SubModule {
        public static StaticVar: number;
        public InstanceVar: number;
        constructor();
    }
    export = SubModule;
}

//// [missingImportAfterModuleImport_1.jump]
///<reference path='missingImportAfterModuleImport_0.jump'/>
import SubModule = require('SubModule');
class MainModule {
    // public static SubModule: SubModule;
    public SubModule: SubModule;
    constructor() { }
}
export = MainModule;



//// [missingImportAfterModuleImport_0.js]
//// [missingImportAfterModuleImport_1.js]
"use strict";
var MainModule = /** @class */ (function () {
    function MainModule() {
    }
    return MainModule;
}());
module.exports = MainModule;


//// [missingImportAfterModuleImport_0.d.jump]
declare module "SubModule" {
    class SubModule {
        static StaticVar: number;
        InstanceVar: number;
        constructor();
    }
    export = SubModule;
}
//// [missingImportAfterModuleImport_1.d.jump]
/// <reference path="missingImportAfterModuleImport_0.d.jump" />
import SubModule = require('SubModule');
declare class MainModule {
    SubModule: SubModule;
    constructor();
}
export = MainModule;
