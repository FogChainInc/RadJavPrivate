/// <reference path="RadJav.d.ts" />
/// <reference path="RadJav.Vector2.d.ts" />
/// <reference path="RadJav.Rectangle.d.ts" />
/// <reference path="RadJav.Font.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class GObject {
            name: string;
            type: string;
            _transform: Rectangle;
            _visible: boolean;
            _zIndex: number;
            _text: string;
            _font: Font;
            _cursor: string;
            _parent: GObject;
            _children: GObject[];
            _html: any;
            _appObj: any;
            access: Account;
            createOnPlatforms: Object;
            onBeforeChildCreated(obj: Object, parent?: any): void;
            onCreated(obj: Object): void;
            _events: ((...args: any[]) => any);
            constructor(obj?: any, text?: string, parent?: GObject, beforeCreatedChild?: boolean);
            create(): Promise<GObject>;
            static parseGObjectXML(xmlTag: RadJav.XML.XMLTag): GObject;
            setFont(font: Font): void;
            getFont(): Font;
            setPosition(x: number | RadJav.Vector2, y?: number): void;
            getPosition(): Vector2;
            getX(): number;
            getY(): number;
            setSize(width: number, height: number): void;
            getSize(): Vector2;
            getWidth(): number;
            getHeight(): number;
            setText(text: string): void;
            getText(): string;
            getParent(): GObject;
            getHTML(): any;
            setVisibility(visible: boolean): void;
            getVisibility(): boolean;
            show(): void;
            hide(): void;
            setEnabled(enabled: boolean): void;
            getEnabled(): boolean;
            on(eventName: string, func: Function): any;
            getHTMLDOM(): any;
        }
    }
}
