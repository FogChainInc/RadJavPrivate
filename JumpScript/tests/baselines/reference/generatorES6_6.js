//// [generatorES6_6.jump]
class C {
  *[Symbol.iterator]() {
    let a = yield 1;
  }
}

//// [generatorES6_6.js]
class C {
    *[Symbol.iterator]() {
        let a = yield 1;
    }
}
