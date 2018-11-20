"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GeneratorClass_1 = require("./GeneratorClass");
const Block_1 = require("./Block");
const fs = require("fs");
/// A generator.
class Generator {
    constructor() {
        this.name = "";
        this.customFiles = {};
        this.filename = "";
        this.filesToParse = [];
        this.classes = [];
    }
    /// Append data to a custom file.
    createCustomFile(file) {
        this.customFiles[file.id] = file;
    }
    /// Append data to a custom file.
    appendToCustomFile(customFileID, data) {
        this.customFiles[customFileID].contents += data;
    }
    /// When generating all files, this will determine the output name used.
    outputFilename(filename) {
        this.filename = filename;
    }
    /// The list of files to parse during generation.
    parseFiles(files) {
        this.filesToParse = files;
    }
    /// Generate bindings and parse files.
    generate() {
        for (let iIdx = 0; iIdx < this.filesToParse.length; iIdx++) {
            let file = this.filesToParse[iIdx];
            let content = fs.readFileSync(file).toString();
            let block = Block_1.Block.findBlock(content);
            eval(block.contents);
        }
        for (let iIdx = 0; iIdx < this.classes.length; iIdx++) {
            let genClass = this.classes[iIdx];
            genClass.generate();
        }
    }
    /// Create a class for either parsing or generating.
    createClass(className) {
        let genClass = new GeneratorClass_1.GeneratorClass();
        this.classes.push(genClass);
        return (genClass);
    }
}
exports.Generator = Generator;
//# sourceMappingURL=Generator.js.map