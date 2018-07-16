/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Vector4 {
        constructor(x: Number, y: Number, z: Number, w: Number);
        x: Number;
        y: Number;
        z: Number;
        w: Number;
        toString(): String;
    }
}
declare function parseVector4(string: string): RadJav.Vector4;
