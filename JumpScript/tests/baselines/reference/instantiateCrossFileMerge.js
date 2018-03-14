//// [tests/cases/compiler/instantiateCrossFileMerge.jump] ////

//// [first.jump]
declare class P<R> {
    constructor(callback: (resolve: (value: R) => void) => void);
}

//// [second.jump]
interface P<R> { }
new P<string>(r => { r('foo') });


//// [first.js]
//// [second.js]
new P(function (r) { r('foo'); });
