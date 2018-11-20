/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class Combobox extends RadJav.GUI.GObject {
            static xmlTag: TagType;
            constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject);
            _items: Combobox.Item[];
            onCreated(): void;
            addItem(item: any): void;
            setItems(items: Combobox.Item[]): void;
            deleteItem(index: Number): void;
            getItem(index: Number): Combobox.Item;
            getItems(): Combobox.Item[];
            getNumItems(): Number;
            clear(): void;
            setSelectedItemIndex(index: Number): void;
            getSelectedItemIndex(): Number;
        }
        namespace Combobox {
            class Item {
                constructor(obj: any);
                name: String;
                text: String;
            }
        }
    }
}
