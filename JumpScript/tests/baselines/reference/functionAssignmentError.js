//// [functionAssignmentError.jump]
var func = function (){return "ONE";};
func = function (){return "ONE";};

//// [functionAssignmentError.js]
var func = function () { return "ONE"; };
func = function () { return "ONE"; };
