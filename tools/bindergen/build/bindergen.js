"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const utils = require("./utils");
const fs = require("fs");
const vm = require("vm");
const path = require("path");
/// Main class for generating and parsing files.
class Bindergen {
    /// Add a generator.
    static addGenerator(generator) {
        Bindergen.generators[generator.name] = generator;
    }
    /// Use a generator.
    static useGenerator(name) {
        let gen = Bindergen.generators[name];
        return (new gen());
    }
    /// Start generating.
    static generate(config) {
        for (let iIdx = 0; iIdx < config.files.length; iIdx++) {
            let file = path.normalize(config.parentDir + "/" + config.files[iIdx]);
            try {
                let content = fs.readFileSync(file).toString();
                const sandbox = { Bindergen: Bindergen };
                vm.createContext(sandbox);
                vm.runInContext(content, sandbox);
            }
            catch (ex) {
                utils.showError(ex.message);
            }
        }
    }
}
/// The generators.
Bindergen.generators = {};
exports.Bindergen = Bindergen;
//# sourceMappingURL=Bindergen.js.map