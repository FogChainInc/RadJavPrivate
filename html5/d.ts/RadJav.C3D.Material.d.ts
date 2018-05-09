/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace C3D {
        class Material {
            constructor(canvas3d: any, obj: any, parent?: any, model?: any);
            protected _name: String;
            protected _material: Object;
            getName(): String;
            static _createMaterials(canvas3d: GUI.Canvas3D, materials: Object[]): any[];
        }
    }
}
