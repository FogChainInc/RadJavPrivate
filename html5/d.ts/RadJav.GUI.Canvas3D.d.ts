/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class Canvas3D extends RadJav.GUI.GObject {
            static xmlTag: TagType;
            constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject);
            protected _renderer: any;
            protected _rendererType: Number;
            protected _currentCamera: any;
            protected _models: Object[];
            protected _materials: object[];
            protected _sceneManager: any;
            create(): Promise<RadJav.GUI.GObject>;
            _setupDefaultCamera(): Promise<RadJav.C3D.Camera>;
            _setupDefaultSceneManager(): void;
            setAmbientLightColor(colour: Color): void;
            createEntity(name: string, parent: RadJav.C3D.Object3D, model: RadJav.C3D.Model): Promise<any>;
            addModel(model: RadJav.C3D.Model): void;
            addMaterial(material: RadJav.C3D.Material): void;
            getNumModels(): number;
            getNumMaterials(): number;
            render(): void;
            createWorld(colour: any): void;
            setWorld(colour: Color): void;
        }
        namespace Canvas3D {
            enum RendererTypes {
                AnyAvailable = 1,
                WebGL = 2,
                Context2D = 3,
            }
        }
    }
}
