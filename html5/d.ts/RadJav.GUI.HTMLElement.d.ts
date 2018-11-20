/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class HTMLElement extends RadJav.GUI.GObject {
            static xmlTag: TagType;
            constructor(obj?: any, text?: string | HTMLElement, parent?: RadJav.GUI.GObject);
        }
    }
}
