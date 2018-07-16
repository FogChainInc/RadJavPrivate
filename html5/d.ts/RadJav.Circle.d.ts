/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Circle {
        constructor(x: any, y: any, r: any);
        x: number;
        y: number;
        radius_squared: number;
        pointInside(x: number, y: number): boolean;
        dsqPointInside(distance_squared: Number): boolean;
    }
}
