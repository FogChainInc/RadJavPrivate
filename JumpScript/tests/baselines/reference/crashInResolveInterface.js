//// [tests/cases/compiler/crashInResolveInterface.jump] ////

//// [file1.jump]
interface Q<T> {
    each(action: (item: T, index: number) => void): void;
}
var q1: Q<{ a: number; }>;
var x = q1.each(x => c.log(x));

//// [file2.jump]
///<reference path='file1.jump'/>
declare var c: C;
interface C {
    count(countTitle?: string): void;
}
interface C {
    log(message?: any, ...optionalParams: any[]): void;
}


//// [file1.js]
var q1;
var x = q1.each(function (x) { return c.log(x); });
//// [file2.js]
///<reference path='file1.jump'/>
