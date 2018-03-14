//// [tests/cases/compiler/exportDefaultParenthesize.jump] ////

//// [commalist.jump]
export default {
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
    ['foo'+'']: 42,
};

//// [comma.jump]
export default {
    ['foo']: 42
};

//// [functionexpression.jump]
export default () => 42;


//// [commalist.js]
export default (_a = {},
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a['foo' + ''] = 42,
    _a);
var _a;
//// [comma.js]
export default (_a = {},
    _a['foo'] = 42,
    _a);
var _a;
//// [functionexpression.js]
export default (function () { return 42; });
