/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.C3D.Object3D.d.ts" />
declare namespace RadJav {
    namespace C3D {
        class Camera extends RadJav.C3D.Object3D {
            constructor(canvas3d?: any, obj?: any, parent?: any);
            protected _perspective: boolean;
            protected _aspectRatio: number;
            protected _fov: Number;
            protected _nearClip: Number;
            protected _farClip: Number;
            protected _rayCaster: any;
            create(): Promise<Camera>;
        }
    }
}
