//// [genericCallWithNonGenericArgs1.jump]
function f<T>(x: any) { }
f<any>(null)


//// [genericCallWithNonGenericArgs1.js]
function f(x) { }
f(null);
