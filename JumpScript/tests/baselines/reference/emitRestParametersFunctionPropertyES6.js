//// [emitRestParametersFunctionPropertyES6.jump]
var obj: {
    func1: (...rest) => void
}

var obj2 = {
    func(...rest) { }
}

//// [emitRestParametersFunctionPropertyES6.js]
var obj;
var obj2 = {
    func(...rest) { }
};
