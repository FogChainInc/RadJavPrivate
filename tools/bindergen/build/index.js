"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const fs = require("fs");
const path = require("path");
const utils = require("./utils");
const Bindergen_1 = require("./Bindergen");
var config = null;
function build(configPath) {
    let parentDir = path.dirname(configPath);
    let configStr = fs.readFileSync(configPath).toString();
    config = JSON.parse(configStr);
    config.parentDir = parentDir;
    for (let iIdx = 0; iIdx < config.generators.length; iIdx++) {
        let generatorPath = config.generators[iIdx];
        try {
            let foundGenPath = "";
            if (fs.existsSync(`${__dirname}/gen/${generatorPath}`) == true)
                foundGenPath = `${__dirname}/gen/${generatorPath}`;
            if (fs.existsSync(`${__dirname}/build/gen/${generatorPath}`) == true)
                foundGenPath = `${__dirname}/build/gen/${generatorPath}`;
            if (fs.existsSync(`${parentDir}/${generatorPath}`) == true)
                foundGenPath = `${parentDir}/${generatorPath}`;
            if (fs.existsSync(`${parentDir}/gen/${generatorPath}`) == true)
                foundGenPath = `${parentDir}/gen/${generatorPath}`;
            if (foundGenPath != "") {
                let generator = require(foundGenPath);
                Bindergen_1.Bindergen.addGenerator(generator);
            }
        }
        catch (ex) {
            utils.showError(ex.message);
        }
    }
    Bindergen_1.Bindergen.generate(config);
}
const commands = [
    {
        cmd: ["build", "b"],
        desc: "Generate the bindings.",
        help: "",
        evt: function () {
            let configPath = path.normalize(__dirname + "/../../../library/gen/config.json");
            build(configPath);
        }
    },
    {
        cmd: ["watch", "w"],
        desc: "Watch for files changes and generate.",
        help: "",
        evt: function (args) {
        }
    }
];
utils.commandLine(commands);
//# sourceMappingURL=index.js.map