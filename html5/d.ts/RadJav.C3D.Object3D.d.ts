/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.C3D.Transform.d.ts" />
declare namespace RadJav {
    namespace C3D {
        class Object3D {
            name: String;
            type: string;
            protected _visible: boolean;
            protected _parent: Object3D;
            protected _canvas3D: GUI.Canvas3D;
            protected _obj3d: any;
            protected _c3dObj: any;
            protected _transform: C3D.Transform;
            constructor(canvas3d: any, obj: any, parent: any);
            getCanvas3D(): GUI.Canvas3D;
            getObj3D(): any;
            create(): Promise<Object3D>;
            destroy(): void;
            getParent(): Object3D;
            getTransform(): C3D.Transform;
            setPosition(x: Number, y: number, z: number): void;
            getPosition(): Vector3;
            setVisibility(visible: boolean): void;
            getVisibility(): boolean;
            show(): void;
            hide(): void;
            on(eventName: String, func: Function): void;
        }
    }
}
