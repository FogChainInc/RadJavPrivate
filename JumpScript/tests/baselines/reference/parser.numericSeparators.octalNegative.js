//// [tests/cases/conformance/parser/ecmascriptnext/numericSeparators/parser.numericSeparators.octalNegative.jump] ////

//// [1.jump]
0o00_

//// [2.jump]
0o_110

//// [3.jump]
0_O0101

//// [4.jump]
0o01__11

//// [5.jump]
0O0110_0110__

//// [6.jump]
0o___0111010_0101_1


//// [1.js]
0;
//// [2.js]
72;
//// [3.js]
0;
O0101;
//// [4.js]
73;
//// [5.js]
294984;
//// [6.js]
1224999433;
