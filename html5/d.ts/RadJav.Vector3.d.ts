/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Vector3 {
        constructor(x?: number | RadJav.Vector2 | RadJav.Vector3, y?: number, z?: number);
        x: number;
        y: number;
        z: number;
        tostring(): string;
        add(vector3: Vector3 | number): Vector3;
        subtract(vector3: Vector3 | number): Vector3;
        multiply(vector3: Vector3 | number): Vector3;
        divide(vector3: Vector3 | number): Vector3;
        dot(vector3: Vector3): number;
        cross(vector3: Vector3): Vector3;
        length(): number;
        normalize(): Vector3;
        squaredLength(): number;
        absDotProduct(vector3: Vector3): number;
        angleBetween(vector3: Vector3): number;
        distance(vector3: Vector3): number;
        static lerp(start: Vector3, end: Vector3, alpha: number): Vector3;
    }
}
