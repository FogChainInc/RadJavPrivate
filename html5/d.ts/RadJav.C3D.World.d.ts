/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace C3D {
        class World {
            constructor(obj?: any);
            _name: string;
            _ambientLightColor: Color;
            _sceneManager: any;
            _renderWindow: any;
            setAmbientLightColor(color: Color): void;
            getAmbientLightColor(): Color;
        }
    }
}
