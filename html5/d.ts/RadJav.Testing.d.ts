/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace Testing {
        class KeyboardSimulator {
            static keyPress(key: string): void;
        }
        class MouseSimulator {
            static click(button: number): void;
            static setPosition(pos: Vector2): void;
        }
        class TouchSimulator {
            static setPosition(pos: Vector2): void;
        }
        class AccelerometerSimulator {
            static shake(pos: Vector2): void;
        }
    }
}
