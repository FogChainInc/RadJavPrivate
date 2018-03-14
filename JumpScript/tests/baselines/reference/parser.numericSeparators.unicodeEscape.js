//// [tests/cases/conformance/parser/ecmascriptnext/numericSeparators/parser.numericSeparators.unicodeEscape.jump] ////

//// [1.jump]
"\u{10_ffff}"

//// [2.jump]
'\u{10_ffff}'

//// [3.jump]
`\u{10_ffff}`

//// [4.jump]
/\u{10_ffff}/u

//// [5.jump]
"\uff_ff"

//// [6.jump]
'\uff_ff'

//// [7.jump]
`\uff_ff`

//// [8.jump]
/\uff_ff/u

//// [9.jump]
"\xf_f"

//// [10.jump]
'\xf_f'

//// [11.jump]
`\xf_f`

//// [12.jump]
/\xf_f/u

//// [13.jump]
"\u{_10ffff}"

//// [14.jump]
'\u{_10ffff}'

//// [15.jump]
`\u{_10ffff}`

//// [16.jump]
/\u{_10ffff}/u

//// [17.jump]
"\u_ffff"

//// [18.jump]
'\u_ffff'

//// [19.jump]
`\u_ffff`

//// [20.jump]
/\u_ffff/u

//// [21.jump]
"\x_ff"

//// [22.jump]
'\x_ff'

//// [23.jump]
`\x_ff`

//// [24.jump]
/\x_ff/u

//// [25.jump]
"\u{10ffff_}"

//// [26.jump]
'\u{10ffff_}'

//// [27.jump]
`\u{10ffff_}`

//// [28.jump]
/\u{10ffff_}/u

//// [29.jump]
"\uffff_"

//// [30.jump]
'\uffff_'

//// [31.jump]
`\uffff_`

//// [32.jump]
/\uffff_/u

//// [33.jump]
"\xff_"

//// [34.jump]
'\xff_'

//// [35.jump]
`\xff_`

//// [36.jump]
/\xff_/u

//// [37.jump]
"\u{10__ffff}"

//// [38.jump]
'\u{10__ffff}'

//// [39.jump]
`\u{10__ffff}`

//// [40.jump]
/\u{10__ffff}/u

//// [41.jump]
"\uff__ff"

//// [42.jump]
'\uff__ff'

//// [43.jump]
`\uff__ff`

//// [44.jump]
/\uff__ff/u

//// [45.jump]
"\xf__f"

//// [46.jump]
'\xf__f'

//// [47.jump]
`\xf__f`

//// [48.jump]
/\xf__f/u


//// [1.js]
"\u{10_ffff}";
//// [2.js]
'\u{10_ffff}';
//// [3.js]
"_ffff}";
//// [4.js]
/\u{10_ffff}/u;
//// [5.js]
"\uff_ff";
//// [6.js]
'\uff_ff';
//// [7.js]
"_ff";
//// [8.js]
/\uff_ff/u;
//// [9.js]
"\xf_f";
//// [10.js]
'\xf_f';
//// [11.js]
"_f";
//// [12.js]
/\xf_f/u;
//// [13.js]
"\u{_10ffff}";
//// [14.js]
'\u{_10ffff}';
//// [15.js]
"_10ffff}";
//// [16.js]
/\u{_10ffff}/u;
//// [17.js]
"\u_ffff";
//// [18.js]
'\u_ffff';
//// [19.js]
"_ffff";
//// [20.js]
/\u_ffff/u;
//// [21.js]
"\x_ff";
//// [22.js]
'\x_ff';
//// [23.js]
"_ff";
//// [24.js]
/\x_ff/u;
//// [25.js]
"\u{10ffff_}";
//// [26.js]
'\u{10ffff_}';
//// [27.js]
"_}";
//// [28.js]
/\u{10ffff_}/u;
//// [29.js]
"\uffff_";
//// [30.js]
'\uffff_';
//// [31.js]
"\uFFFF_";
//// [32.js]
/\uffff_/u;
//// [33.js]
"\xff_";
//// [34.js]
'\xff_';
//// [35.js]
"\u00FF_";
//// [36.js]
/\xff_/u;
//// [37.js]
"\u{10__ffff}";
//// [38.js]
'\u{10__ffff}';
//// [39.js]
"__ffff}";
//// [40.js]
/\u{10__ffff}/u;
//// [41.js]
"\uff__ff";
//// [42.js]
'\uff__ff';
//// [43.js]
"__ff";
//// [44.js]
/\uff__ff/u;
//// [45.js]
"\xf__f";
//// [46.js]
'\xf__f';
//// [47.js]
"__f";
//// [48.js]
/\xf__f/u;
