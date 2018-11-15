/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Vector4 {
        constructor(x?: number | RadJav.Vector2 | RadJav.Vector3 | RadJav.Vector4, y?: number, z?: number, w?: number);
        x: number;
        y: number;
        z: number;
        w: number;
        tostring(): string;
        lerp(start: Vector4, end: Vector4, time: number): Vector4;
    }
}
declare function parseVector4(string: string): RadJav.Vector4;
