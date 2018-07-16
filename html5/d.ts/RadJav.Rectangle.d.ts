/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Rectangle {
        constructor(x?: any, y?: number, w?: number, h?: number);
        x: number;
        y: number;
        width: number;
        height: number;
        top: number;
        bottom: number;
        left: number;
        right: number;
        setPosition(x: any, y?: number): void;
        getPosition(): Vector2;
        setSize(w: any, h?: number): void;
        getSize(): Vector2;
        pointInside(x: any, y: Number): boolean;
        xInside(x: Number): boolean;
        yInside(y: Number): boolean;
    }
}
