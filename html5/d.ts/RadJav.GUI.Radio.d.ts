/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class Radio extends RadJav.GUI.GObject {
            static xmlTag: TagType;
            constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject);
            _radioGroup: String;
            _checked: boolean;
            setChecked(checked: boolean): void;
            isChecked(): boolean;
        }
    }
}
