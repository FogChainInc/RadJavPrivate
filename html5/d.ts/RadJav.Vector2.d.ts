/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Vector2 {
        constructor(x?: number | Vector2, y?: number);
        x: number;
        y: number;
        tostring(): string;
        setX(n: number): void;
        setY(n: number): void;
        set(x: number, y: number): void;
        add(x: number, y: number): void;
        sub(subVector2: Vector2): void;
        mult(n: number): void;
        divide(vector2: number | Vector2): Vector2;
        dot(dotVector2: Vector2): number;
        length(): number;
        normalize(): Vector2;
        static parseVector2(str: string): Vector2;
    }
}
