/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    /** @class RadJav.Testing
    * The testing class.
    * Available on platforms: Windows,Linux,OSX,iOS,Android
    */
    namespace Testing {
        /** Simulates keyboard input.
        * Available on platforms: Windows,Linux,OSX,iOS,Android
        */
        class KeyboardSimulator {
            static keyPress(key: string): void;
        }
        /** Simulates mouse input.
        * Available on platforms: Windows,Linux,OSX,iOS,Android
        */
        class MouseSimulator {
            static click(button: number): void;
            static setPosition(pos: Vector2): void;
        }
        /** Simulates touch input.
        * Available on platforms: Windows,Linux,OSX,iOS,Android
        */
        class TouchSimulator {
            static setPosition(pos: Vector2): void;
        }
        /** Simulates accelerometer input.
        * Available on platforms: iOS,Android
        */
        class AccelerometerSimulator {
            static shake(pos: Vector2): void;
        }
    }
}
