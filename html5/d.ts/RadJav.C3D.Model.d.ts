/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace C3D {
        class Model {
            constructor(object3d: any, obj: any, materials: any);
            protected _object3d: RadJav.C3D.Object3D;
            protected _name: string;
            protected mesh: Model.Mesh;
            protected materials: RadJav.C3D.Material[];
            create(): Promise<Model>;
            protected _setName(name: string): void;
            getName(): string;
        }
        namespace Model {
            class Mesh {
                constructor(model: any, obj: any);
                _name: String;
                filePath: String;
                type: String;
                data: Mesh.Data | object;
                _mesh: Object;
                model: Model;
                getName(): String;
                create(): Promise<{}>;
            }
            namespace Mesh {
                class Data {
                    constructor(mesh: any, obj: any);
                    type: 'mesh' | 'sphere' | 'cube' | 'plane';
                    radius: Number;
                    width: Number;
                    height: Number;
                    depth: Number;
                    widthSegments: Number;
                    heightSegments: Number;
                    depthSegments: Number;
                }
            }
        }
    }
}
