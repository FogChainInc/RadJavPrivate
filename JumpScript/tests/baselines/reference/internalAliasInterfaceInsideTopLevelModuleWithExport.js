//// [internalAliasInterfaceInsideTopLevelModuleWithExport.jump]
export module a {
    export interface I {
    }
}

export import b = a.I;
export var x: b;


//// [internalAliasInterfaceInsideTopLevelModuleWithExport.js]
"use strict";
exports.__esModule = true;


//// [internalAliasInterfaceInsideTopLevelModuleWithExport.d.jump]
export declare module a {
    interface I {
    }
}
export import b = a.I;
export declare var x: b;
