var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
var RadJav;
(function (RadJav) {
    var Animation = (function () {
        function Animation() {
            this.name = "";
            this.attachedObject = null;
            this.lerpAnimators = [];
            this.onUpdate = null;
            this.playState = Animation.PlayState.Stopped;
            RadJav.addAnimation(this);
        }
        Animation.prototype.attach = function (obj) {
            this.attachedObject = obj;
        };
        Animation.prototype._getDisplayObject = function (obj) {
            var foundObj = null;
            if (this.attachedObject.constructor["name"] == "HTMLElement")
                foundObj = this.attachedObject;
            if (this.attachedObject.constructor["name"] == "Component")
                foundObj = this._getDisplayObject(this.attachedObject.display);
            if (this.attachedObject instanceof RadJav.GUI.GObject)
                foundObj = this.attachedObject;
            if (this.attachedObject instanceof RadJav.C3D.Object3D)
                foundObj = this.attachedObject;
            return (foundObj);
        };
        Animation.prototype.lerp = function (speed, x, y, z) {
            if (y === void 0) { y = 0; }
            if (z === void 0) { z = 0; }
            var start = null;
            var end = new RadJav.Vector3();
            var foundObj = this._getDisplayObject(this.attachedObject);
            if (foundObj.constructor["name"] == "HTMLElement")
                start = new RadJav.Vector3(this.attachedObject.offsetLeft, this.attachedObject.offsetTop, 0);
            if (foundObj.constructor["name"] == "GObject") {
                var pos = this.attachedObject.getPosition();
                start = new RadJav.Vector3(pos);
            }
            if (foundObj.constructor["name"] == "Object3D")
                start = this.attachedObject.getPosition();
            if (typeof (x) == "number") {
                end.x = x;
                end.y = y;
                end.z = z;
            }
            else {
                if (x.constructor["name"] == "Vector2")
                    end = new RadJav.Vector3(x);
                if (x.constructor["name"] == "Vector3")
                    end = new RadJav.Vector3(x);
            }
            var animator = new Animation.LerpAnimator(foundObj, start, end, speed);
            animator.startTime = RadJav.getTime();
            animator.onAnimate = function (animObj, newPos, timeDelta) {
                if (animObj.constructor["name"] == "HTMLElement") {
                    animObj.style.left = newPos.x + "px";
                    animObj.style.top = newPos.y + "px";
                }
                else {
                    if (animObj instanceof RadJav.GUI.GObject)
                        animObj.setPosition(newPos);
                    if (animObj instanceof RadJav.C3D.Object3D)
                        animObj.setPosition(newPos);
                }
            };
            this.lerpAnimators.push(animator);
        };
        Animation.prototype.play = function () {
            for (var iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++) {
                var animation = this.lerpAnimators[iIdx];
                animation.startTime = RadJav.getTime();
            }
            this.playState = Animation.PlayState.Playing;
        };
        Animation.prototype.pause = function () {
            for (var iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++) {
                var animation = this.lerpAnimators[iIdx];
            }
            this.playState = Animation.PlayState.Paused;
        };
        Animation.prototype.stop = function () {
            for (var iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++) {
                var animation = this.lerpAnimators[iIdx];
            }
            this.playState = Animation.PlayState.Stopped;
        };
        Animation.prototype.on = function (event, func) {
            if (event == "update")
                this.onUpdate = func;
        };
        Animation.prototype.update = function (timeDelta) {
            if (this.playState != Animation.PlayState.Playing)
                return;
            for (var iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++) {
                var animation = this.lerpAnimators[iIdx];
                animation.update(timeDelta);
            }
            if (this.onUpdate != null)
                this.onUpdate(timeDelta);
        };
        return Animation;
    }());
    RadJav.Animation = Animation;
    (function (Animation) {
        var Animator = (function () {
            function Animator(attachedObject) {
                this.attachedObject = attachedObject;
                this.onAnimate = null;
            }
            Animator.prototype.animate = function (data, timeDelta) {
                if (this.onAnimate != null)
                    this.onAnimate(this.attachedObject, data, timeDelta);
            };
            return Animator;
        }());
        Animation.Animator = Animator;
        var LerpAnimator = (function (_super) {
            __extends(LerpAnimator, _super);
            function LerpAnimator(attachedObject, start, end, speed) {
                var _this = _super.call(this, attachedObject) || this;
                _this.startPos = start;
                _this.endPos = end;
                _this.speed = speed;
                _this.startTime = 0;
                _this.currentTime = 0;
                _this.length = end.distance(start);
                return _this;
            }
            LerpAnimator.prototype.update = function (timeDelta) {
                var currentPos = new RadJav.Vector3();
                var posMoved = ((RadJav.getTime() - this.startTime) * this.speed);
                var posDelta = posMoved / this.length;
                currentPos = currentPos.lerp(this.startPos, this.endPos, posDelta);
                this.animate(currentPos, timeDelta);
            };
            return LerpAnimator;
        }(Animator));
        Animation.LerpAnimator = LerpAnimator;
        var PlayState = (function () {
            function PlayState() {
            }
            PlayState.Stopped = 1;
            PlayState.Playing = 2;
            PlayState.Paused = 3;
            return PlayState;
        }());
        Animation.PlayState = PlayState;
    })(Animation = RadJav.Animation || (RadJav.Animation = {}));
})(RadJav || (RadJav = {}));
