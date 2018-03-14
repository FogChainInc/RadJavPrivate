//// [computerPropertiesInES5ShouldBeTransformed.jump]
const b = ({ [`key`]: renamed }) => renamed;

//// [computerPropertiesInES5ShouldBeTransformed.js]
var b = function (_a) {
    var _b = "key", renamed = _a[_b];
    return renamed;
};
