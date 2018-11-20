/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class Checkbox extends RadJav.GUI.GObject {
            static xmlTag: TagType;
            constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject);
            _checked: boolean;
            setChecked(checked: boolean): void;
            isChecked(): boolean;
        }
    }
}
