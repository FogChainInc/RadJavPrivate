//// [tests/cases/conformance/parser/ecmascriptnext/numericSeparators/parser.numericSeparators.hexNegative.jump] ////

//// [1.jump]
0x00_

//// [2.jump]
0x_110

//// [3.jump]
0_X0101

//// [4.jump]
0x01__11

//// [5.jump]
0X0110_0110__

//// [6.jump]
0x___0111010_0101_1


//// [1.js]
0;
//// [2.js]
272;
//// [3.js]
0;
X0101;
//// [4.js]
273;
//// [5.js]
17826064;
//// [6.js]
1172542853137;
