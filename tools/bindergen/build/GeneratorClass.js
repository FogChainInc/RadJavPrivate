"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/// A class to be used for generating bindings or parsing.
class GeneratorClass {
    constructor() {
        this.name = "";
        this.extends = "";
        this.functions = [];
        this.parseEvents = {};
        this.references = [];
    }
    /// During generation, refer to a reference for something.
    referFrom(reference) {
        this.references.push(reference);
    }
    /// Create a parse event.
    parse(event, func) {
        this.parseEvents[event] = func;
    }
    /// Generate bindings and parse files.
    generate() {
    }
}
exports.GeneratorClass = GeneratorClass;
//# sourceMappingURL=GeneratorClass.js.map