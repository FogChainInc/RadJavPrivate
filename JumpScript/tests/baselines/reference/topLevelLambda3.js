//// [topLevelLambda3.jump]
var f = () => {this.window;}

//// [topLevelLambda3.js]
var _this = this;
var f = function () { _this.window; };
