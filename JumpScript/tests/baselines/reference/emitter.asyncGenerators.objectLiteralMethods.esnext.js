//// [tests/cases/conformance/emitter/esnext/asyncGenerators/emitter.asyncGenerators.objectLiteralMethods.esnext.jump] ////

//// [O1.jump]
const o1 = {
    async * f() {
    }
}
//// [O2.jump]
const o2 = {
    async * f() {
        const x = yield;
    }
}
//// [O3.jump]
const o3 = {
    async * f() {
        const x = yield 1;
    }
}
//// [O4.jump]
const o4 = {
    async * f() {
        const x = yield* [1];
    }
}
//// [O5.jump]
const o5 = {
    async * f() {
        const x = yield* (async function*() { yield 1; })();
    }
}
//// [O6.jump]
const o6 = {
    async * f() {
        const x = await 1;
    }
}
//// [O7.jump]
const o7 = {
    async * f() {
        return 1;
    }
}


//// [O1.js]
const o1 = {
    async *f() {
    }
};
//// [O2.js]
const o2 = {
    async *f() {
        const x = yield;
    }
};
//// [O3.js]
const o3 = {
    async *f() {
        const x = yield 1;
    }
};
//// [O4.js]
const o4 = {
    async *f() {
        const x = yield* [1];
    }
};
//// [O5.js]
const o5 = {
    async *f() {
        const x = yield* (async function* () { yield 1; })();
    }
};
//// [O6.js]
const o6 = {
    async *f() {
        const x = await 1;
    }
};
//// [O7.js]
const o7 = {
    async *f() {
        return 1;
    }
};
