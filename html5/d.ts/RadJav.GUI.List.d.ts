/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class List extends RadJav.GUI.GObject {
            constructor(obj?: any, text?: String, parent?: RadJav.GUI.GObject);
            protected _canSort: any;
            protected _hasCheckBoxes: any;
            protected _columns: any;
            addColumn(column: any, width: number, key: any): void;
            setColumns(columns: any): void;
            addRow(row: any, hiddenValue: any): void;
            setRows(rows: any, hiddenRows: any): void;
            getSelectedRows(): any;
            deleteRows(selection: any): any;
        }
        class Row {
            constructor(obj: any);
            items: any;
            addItem(item: any): void;
        }
        namespace List {
            class Item {
                constructor(obj: any);
                name: String;
                text: string;
            }
            class Column {
                constructor(obj: any);
                text: String;
                width: number;
                key: object;
            }
            class Selection {
                constructor(obj: any);
                _html: any;
                _appObj: any;
            }
        }
    }
}
