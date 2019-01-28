/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        /** @class RadJav.GUI.Combobox
         * @extends RadJav.GUI.GObject
         * A combobox.
         * Available on platforms: Windows,Linux,OSX,HTML5
         */
        class Canvas3D extends RadJav.GUI.GObject {
            static xmlTag: TagType;
            constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject);
            /** @property {Mixed} [_renderer=null]
             * @protected
             * The renderer used to render the canvas.
             */
            protected _renderer: any;
            /** @property {Number} [_rendererType=1]
             * @protected
             * The renderer type used to render the canvas.
             */
            protected _rendererType: Number;
            /** @property {Object} [_currentCamera=null]
             * @protected
             * The current camera used to render the scene.
             */
            protected _currentCamera: any;
            /** @property {Object} [_models={}]
             * @protected
             * The models that have been loaded for use. Each key is a RadJav.C3D.Model.
             */
            protected _models: Object[];
            /** @property {Object} [_materials={}]
             * @protected
             * The materials that have been loaded for use. Each key is a RadJav.C3D.Material.
             */
            protected _materials: object[];
            protected _sceneManager: any;
            create(): Promise<RadJav.GUI.GObject>;
            /** @method _setupDefaultCamera
             * @protected
             * Setup the default camera.
             * @return {Promise} The promise to execute when the camera has finished being
             * created.
             */
            _setupDefaultCamera(): Promise<RadJav.C3D.Camera>;
            /** @method _setupDefaultSceneManager
             * @protected
             * Setup the default scene manager.
             * @return {RadJav.GUI.GObject} The parent of this object.
             */
            _setupDefaultSceneManager(): void;
            /** @method setAmbientLightColor
             * Set the ambient light color of the scene.
             * @param {RadJav.Color} color The color.
             */
            setAmbientLightColor(colour: Color): void;
            /** @method createEntity
             * Create an entity to display in the scene.
             * @param {String} name The name of the object.
             * @param {RadJav.C3D.Object3D} parent The parent object.
             * @param {RadJav.C3D.Model} model The 3d model to create.
             * @return {Promise} The promise to execute when the entity has finished creating.
             */
            createEntity(name: string, parent: RadJav.C3D.Object3D, model: RadJav.C3D.Model): Promise<any>;
            /** @method addModel
             * Add a loaded model for use.
             * @param {RadJav.C3D.Model} model The model to add.
             */
            addModel(model: RadJav.C3D.Model): void;
            /** @method addMaterial
             * Add a loaded material for use.
             * @param {RadJav.C3D.Material} material The material to add.
             */
            addMaterial(material: RadJav.C3D.Material): void;
            /** @method getNumModels
             * Get the number of models that have been loaded.
             */
            getNumModels(): number;
            /** @method getNumMaterials
             * Get the number of materials that have been loaded.
             */
            getNumMaterials(): number;
            /** @method render
             * Perform the actual rendering.
             */
            render(): void;
            /** @method createWorld
             * Set the ambient light color of the scene.
             * @param {RadJav.Color} color The color.
             */
            createWorld(colour: any): void;
            /** @method createWorld
             * Set the ambient light color of the scene.
             * @param {RadJav.Color} color The color.
             */
            setWorld(colour: Color): void;
        }
        namespace Canvas3D {
            /** A 3d canvas.
             * Available on platforms: Windows,Linux,OSX,HTML5
             */
            enum RendererTypes {
                AnyAvailable = 1,
                WebGL = 2,
                Context2D = 3,
            }
        }
    }
}
