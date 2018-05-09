/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.C3D.Object3D.d.ts" />
declare namespace RadJav {
    namespace C3D {
        class Entity extends RadJav.C3D.Object3D {
            constructor(canvas3d: any, obj: any, parent: any, model: any);
            protected _model: any;
            protected _c3dEntity: Object;
            create(): Promise<Entity>;
            setModel(newModel: string): void;
            getModel(): any;
            Types: {
                None: number;
                Cube: number;
                Sphere: number;
                Plane: number;
                Camera: number;
                Light: number;
            };
        }
    }
}
