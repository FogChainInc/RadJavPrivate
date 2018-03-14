//// [genericInference1.jump]
['a', 'b', 'c'].map(x => x.length);

//// [genericInference1.js]
['a', 'b', 'c'].map(function (x) { return x.length; });
