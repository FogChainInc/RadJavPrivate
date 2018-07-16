/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace GUI {
        class GObject {
            name: String;
            type: string;
            _transform: Rectangle;
            _visible: boolean;
            _zIndex: Number;
            _text: String;
            _font: Font;
            _cursor: String;
            _parent: GObject;
            _children: GObject[];
            _html: any;
            _appObj: any;
            createOnPlatforms: Object;
            onBeforeChildCreated(obj: Object, parent?: any): void;
            onCreated(obj: Object): void;
            _events: ((...args: any[]) => any);
            constructor(obj?: any, text?: String, parent?: GObject, beforeCreatedChild?: boolean);
            create(): Promise<GObject>;
            setFont(font: Font): void;
            getFont(): Font;
            setPosition(x: Number, y: number): void;
            getPosition(): Vector2;
            getX(): Number;
            getY(): Number;
            setSize(width: Number, height: number): void;
            getSize(): Vector2;
            getWidth(): Number;
            getHeight(): Number;
            setText(text: String): void;
            getText(): String;
            getParent(): GObject;
            getHTML(): any;
            setVisibility(visible: boolean): void;
            getVisibility(): boolean;
            show(): void;
            hide(): void;
            setEnabled(enabled: boolean): void;
            getEnabled(): boolean;
            on(eventName: String, func: ((...args: any[]) => any)): any;
            getHTMLDOM(): any;
        }
    }
}
