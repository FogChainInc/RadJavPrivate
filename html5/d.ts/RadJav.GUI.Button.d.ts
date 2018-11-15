/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class Button extends RadJav.GUI.GObject {
            constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject);
            static xmlTag: RadJav.TagType;
        }
    }
}
