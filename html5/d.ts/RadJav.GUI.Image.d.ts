/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class Image extends RadJav.GUI.GObject {
            static xmlTag: TagType;
            constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject);
            protected _image: any;
            onCreated(): void;
            setImage(image: string | any): void;
        }
    }
}
