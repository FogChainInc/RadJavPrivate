/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Vector3 {
        constructor(x?: any, y?: any, z?: any);
        x: number;
        y: number;
        z: number;
        toString(): String;
        add(vector3: Vector3): Vector3;
        subtract(vector3: Vector3): Vector3;
        multiply(vector3: Vector3): Vector3;
        divide(vector3: Vector3 | number): Vector3;
        dot(vector3: Vector3): number;
        cross(vector3: Vector3): Vector3;
        length(): number;
        normalize(): number | Vector3;
        squaredLength(): Number;
        absDotProduct(vector3: Vector3): Number;
        angleBetween(vector3: Vector3): number;
        distance(vector3: Vector3): Number;
    }
}
