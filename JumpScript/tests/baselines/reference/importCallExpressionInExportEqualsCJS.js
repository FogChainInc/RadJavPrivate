//// [tests/cases/conformance/dynamicImport/importCallExpressionInExportEqualsCJS.jump] ////

//// [something.jump]
export = 42;

//// [index.jump]
export = async function() {
    const something = await import("./something");
};

//// [something.js]
"use strict";
module.exports = 42;
//// [index.js]
"use strict";
module.exports = async function () {
    const something = await Promise.resolve().then(() => require("./something"));
};
