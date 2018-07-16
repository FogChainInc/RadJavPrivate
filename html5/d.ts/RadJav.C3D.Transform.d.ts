/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace C3D {
        class Transform {
            constructor(object3d: any, obj?: any, position?: any);
            protected position: any;
            _object3d: Object3D;
            _parent: Transform;
            _sceneNode: any;
            _movable: any;
            addChild(child: Object3D): void;
            setPosition(x: any | Vector3, y: number, z: number): void;
            getPosition(): Vector3;
        }
    }
}
