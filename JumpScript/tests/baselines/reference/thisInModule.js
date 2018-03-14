//// [thisInModule.jump]
module myMod {
    var x;
    this.x = 5;
}

//// [thisInModule.js]
var myMod;
(function (myMod) {
    var x;
    this.x = 5;
})(myMod || (myMod = {}));
