var RadJav;
(function (RadJav) {
    var Animation = (function () {
        function Animation() {
            this.name = "";
            this.attachedObject = null;
            this.lerpAnimators = [];
            this.onUpdate = null;
            this.playState = PlayState.Stopped;
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
            if (this.attachedObject.constructor["name"] == "GObject")
                foundObj = this.attachedObject;
            if (this.attachedObject.constructor["name"] == "Object3D")
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
                var pos = this.attachedObject.display.getPosition();
                start = new RadJav.Vector3(pos);
            }
            if (foundObj.constructor["name"] == "Object3D")
                start = this.attachedObject.display.getPosition();
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
            var animator = new LerpAnimator(foundObj, start, end, speed);
            animator.startTime = RadJav.getTime();
            animator.onAnimate = function (animObj, newPos, timeDelta) {
                if (animObj.constructor["name"] == "HTMLElement") {
                    animObj.offsetLeft = newPos.x;
                    animObj.offsetTop = newPos.y;
                }
                else
                    animObj.setPosition(newPos);
            };
            this.lerpAnimators.push(animator);
        };
        Animation.prototype.play = function () {
            for (var iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++) {
                var animation = this.lerpAnimators[iIdx];
                animation.startTime = RadJav.getTime();
            }
            this.playState = PlayState.Playing;
        };
        Animation.prototype.pause = function () {
            for (var iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++) {
                var animation = this.lerpAnimators[iIdx];
            }
            this.playState = PlayState.Paused;
        };
        Animation.prototype.stop = function () {
            for (var iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++) {
                var animation = this.lerpAnimators[iIdx];
            }
            this.playState = PlayState.Stopped;
        };
        Animation.prototype.on = function (event, func) {
            if (event == "update")
                this.onUpdate = func;
        };
        Animation.prototype.update = function (timeDelta) {
            if (this.playState != PlayState.Playing)
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
    var LerpAnimator = (function () {
        function LerpAnimator(attachedObject, start, end, speed) {
            this.attachedObject = attachedObject;
            this.onAnimate = null;
            this.startPos = start;
            this.endPos = end;
            this.speed = speed;
            this.startTime = 0;
            this.currentTime = 0;
            this.length = (end - start);
        }
        LerpAnimator.prototype.animate = function (data, timeDelta) {
            if (this.onAnimate != null)
                this.onAnimate(this.attachedObject, data, timeDelta);
        };
        LerpAnimator.prototype.update = function (timeDelta) {
            var currentPos = new RadJav.Vector3();
            var posMoved = ((RadJav.getTime() - this.startTime) * this.speed);
            var posDelta = posMoved / this.length;
            currentPos = currentPos.lerp(this.startPos, this.endPos, posDelta);
            this.animate(currentPos, timeDelta);
        };
        return LerpAnimator;
    }());
    RadJav.LerpAnimator = LerpAnimator;
    var PlayState = (function () {
        function PlayState() {
        }
        PlayState.Stopped = 0;
        PlayState.Playing = 1;
        PlayState.Paused = 2;
        return PlayState;
    }());
    RadJav.PlayState = PlayState;
})(RadJav || (RadJav = {}));
