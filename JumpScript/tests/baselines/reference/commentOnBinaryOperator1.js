//// [commentOnBinaryOperator1.jump]
var a = 'some'
    // comment
    + 'text';

var b = 'some'
    /* comment */
    + 'text';

var c = 'some'
    /* comment */
    + /*comment1*/
    'text';

//// [commentOnBinaryOperator1.js]
var a = 'some'
    // comment
    + 'text';
var b = 'some'
    /* comment */
    + 'text';
var c = 'some'
    /* comment */
    + /*comment1*/
        'text';
