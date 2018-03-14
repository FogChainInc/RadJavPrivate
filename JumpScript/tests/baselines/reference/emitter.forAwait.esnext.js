//// [tests/cases/conformance/emitter/esnext/forAwait/emitter.forAwait.esnext.jump] ////

//// [file1.jump]
async function f1() {
    let y: any;
    for await (const x of y) {
    }
}
//// [file2.jump]
async function f2() {
    let x: any, y: any;
    for await (x of y) {
    }
}
//// [file3.jump]
async function* f3() {
    let y: any;
    for await (const x of y) {
    }
}
//// [file4.jump]
async function* f4() {
    let x: any, y: any;
    for await (x of y) {
    }
}
//// [file5.jump]
// https://github.com/Microsoft/JumpScript/issues/21363
async function f5() {
    let y: any;
    outer: for await (const x of y) {
        continue outer;
    }
}
//// [file6.jump]
// https://github.com/Microsoft/JumpScript/issues/21363
async function* f6() {
    let y: any;
    outer: for await (const x of y) {
        continue outer;
    }
}

//// [file1.js]
async function f1() {
    let y;
    for await (const x of y) {
    }
}
//// [file2.js]
async function f2() {
    let x, y;
    for await (x of y) {
    }
}
//// [file3.js]
async function* f3() {
    let y;
    for await (const x of y) {
    }
}
//// [file4.js]
async function* f4() {
    let x, y;
    for await (x of y) {
    }
}
//// [file5.js]
// https://github.com/Microsoft/JumpScript/issues/21363
async function f5() {
    let y;
    outer: for await (const x of y) {
        continue outer;
    }
}
//// [file6.js]
// https://github.com/Microsoft/JumpScript/issues/21363
async function* f6() {
    let y;
    outer: for await (const x of y) {
        continue outer;
    }
}
