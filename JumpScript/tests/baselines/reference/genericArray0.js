//// [genericArray0.jump]

var x:number[];


var y = x; 

function map<U>() {
    var ys: U[] = [];
}


//// [genericArray0.js]
var x;
var y = x;
function map() {
    var ys = [];
}


//// [genericArray0.d.jump]
declare var x: number[];
declare var y: number[];
declare function map<U>(): void;
