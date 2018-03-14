//// [tests/cases/conformance/parser/ecmascriptnext/numericSeparators/parser.numericSeparators.binaryNegative.jump] ////

//// [1.jump]
0b00_

//// [2.jump]
0b_110

//// [3.jump]
0_B0101

//// [4.jump]
0b01__11

//// [5.jump]
0B0110_0110__

//// [6.jump]
0b___0111010_0101_1


//// [1.js]
0;
//// [2.js]
6;
//// [3.js]
0;
B0101;
//// [4.js]
7;
//// [5.js]
102;
//// [6.js]
1867;
