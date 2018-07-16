/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Color {
        constructor(r?: number | {
            r: 0;
            g: 0;
            b: 0;
            a: 1;
        }, g?: number, b?: number, a?: number);
        r: Number;
        g: Number;
        b: Number;
        a: Number;
        toHex(): string;
        toHTMLColor(): String;
        toHexInt(): Number;
        static Black: Color;
        static White: Color;
        static Red: Color;
        static Green: Color;
        static Blue: Color;
    }
}
declare function parseColor(str: String): RadJav.Color;
