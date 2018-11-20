"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/// The type of path.
var ReferPathType;
(function (ReferPathType) {
    ReferPathType[ReferPathType["URL"] = 0] = "URL";
    ReferPathType[ReferPathType["FILE"] = 1] = "FILE";
})(ReferPathType = exports.ReferPathType || (exports.ReferPathType = {}));
/// References used.
class GeneratorReference {
    constructor() {
        this.name = "";
        this.path = "";
        this.reference = ReferPathType.FILE;
        this.events = {};
    }
    /// Call when an event has occurred.
    on(event, func) {
        this.events[event] = func;
    }
}
exports.GeneratorReference = GeneratorReference;
//# sourceMappingURL=GeneratorReference.js.map