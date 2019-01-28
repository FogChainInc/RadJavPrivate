/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        /** @class RadJav.GUI.Image
         * @extends RadJav.GUI.GObject
         * An image.
         * Available on platforms: Windows,Linux,OSX,HTML5
         */
        class Image extends RadJav.GUI.GObject {
            static xmlTag: TagType;
            constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject);
            /** @property {String/Image} [_image=null]
             * @protected
             * The image thats being used. If a string, it will be converted into
             * an Image when the image is set.
             */
            protected _image: any;
            onCreated(): void;
            /** @method setImage
             * Set the image.
             * Theme Event: setImage
             * Is Theme Event Asynchronous: Yes
             * @param {String/Image} image The image to display.
             * @return {Promise} Executes the promise when the image has loaded.
             */
            setImage(image: string | any): void;
        }
    }
}
