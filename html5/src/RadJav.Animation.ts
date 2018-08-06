/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
	and associated documentation files (the "Software"), to deal in the Software without restriction, 
	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial 
	portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
	LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/// <reference path="RadJav.ts" />
/// <reference path="RadJav.GUI.GObject.ts" />
/// <reference path="RadJav.C3D.Object3D.ts" />
/// <reference path="RadJav.Interact.ts" />
/// <reference path="RadJav.Vector2.ts" />
/// <reference path="RadJav.Vector3.ts" />

namespace RadJav
{
	/// Handles how a UI element is treated.
	export class Animation
	{
		/// Name of the animation.
		public name: string;
		public attachedObject: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D;
		public lerpAnimators: Animation.LerpAnimator[];
		public onUpdate: (timeDelta: number) => void;
		public onCompleted: () => void;
		public playState: Animation.PlayState;

		constructor ()
		{
			this.name = "";
			this.attachedObject = null;
			this.lerpAnimators = [];
			this.onUpdate = null;
			this.onCompleted = null;
			this.playState = Animation.PlayState.Stopped;
			RadJav.addAnimation (this);
		}

		attach (obj: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D): void
		{
			this.attachedObject = obj;
		}

		_getDisplayObject (obj: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D)
								: HTMLElement | RadJav.GUI.GObject | RadJav.C3D.Object3D
		{
			let foundObj: HTMLElement | RadJav.GUI.GObject | RadJav.C3D.Object3D = null;

			if (typeof (HTMLElement) != "undefined")
			{
				if (this.attachedObject instanceof HTMLElement)
					foundObj = (<HTMLElement>this.attachedObject);
			}

			if (typeof (RadJav.Interact) != "undefined")
			{
				if (this.attachedObject instanceof RadJav.Interact.Component)
					foundObj = this._getDisplayObject ((<RadJav.Interact.Component>this.attachedObject).display);
			}

			if (typeof (RadJav.GUI) != "undefined")
			{
				if (this.attachedObject instanceof RadJav.GUI.GObject)
					foundObj = (<RadJav.GUI.GObject>this.attachedObject);
			}

			if (typeof (RadJav.C3D) != "undefined")
			{
				if (typeof (RadJav.C3D.Object3D) != "undefined")
				{
					if (this.attachedObject instanceof RadJav.C3D.Object3D)
						foundObj = (<RadJav.C3D.Object3D>this.attachedObject);
				}
			}

			return (foundObj);
		}

		lerp (start: RadJav.Vector2 | RadJav.Vector3, end: RadJav.Vector2 | RadJav.Vector3, speed: number): void
		{
			let foundObj: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D = this._getDisplayObject (this.attachedObject);

			if (start instanceof RadJav.Vector2)
				start = new RadJav.Vector3 (start);

			if (end instanceof RadJav.Vector2)
				end = new RadJav.Vector3 (end);

			let animator = new Animation.LerpAnimator (this, (<RadJav.Vector3>start), (<RadJav.Vector3>end), speed);
			animator.onAnimate = function (animObj, newPos, timeDelta)
				{
					let execRadJavObj: boolean = true;

					if (typeof (HTMLElement) != "undefined")
					{
						if (animObj instanceof HTMLElement)
						{
							(<HTMLElement>animObj).style.left = newPos.x + "px";
							(<HTMLElement>animObj).style.top = newPos.y + "px";
							execRadJavObj = false;
						}
					}

					if (execRadJavObj == true)
					{
						if (animObj instanceof RadJav.GUI.GObject)
							(<RadJav.GUI.GObject>animObj).setPosition (newPos);

						if (typeof (RadJav.C3D.Object3D) != "undefined")
						{
							if (animObj instanceof RadJav.C3D.Object3D)
								(<RadJav.C3D.Object3D>animObj).setPosition (newPos);
						}
					}
				};
			this.lerpAnimators.push (animator);
		}

		play (): void
		{
			if (this.playState == Animation.PlayState.Playing)
				return;

			for (let iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++)
			{
				let animation: Animation.LerpAnimator = this.lerpAnimators[iIdx];

				animation.startTime = RadJav.getTime ();
				animation.timeElapsed = RadJav.getTime ();
			}

			this.playState = Animation.PlayState.Playing;
		}

		pause (): void
		{
			if (this.playState == Animation.PlayState.Paused)
				return;

			for (let iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++)
			{
				let animation: Animation.LerpAnimator = this.lerpAnimators[iIdx];
			}

			this.playState = Animation.PlayState.Paused;
		}

		stop (): void
		{
			if (this.playState == Animation.PlayState.Stopped)
				return;

			for (let iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++)
			{
				let animation: Animation.LerpAnimator = this.lerpAnimators[iIdx];
			}

			this.playState = Animation.PlayState.Stopped;
		}

		on (event: string, func: (timeDelta?: number) => void)
		{
			if (event == "update")
				this.onUpdate = func;

			if (event == "completed")
				this.onCompleted = func;
		}

		update (timeDelta: number): void
		{
			if (this.playState != Animation.PlayState.Playing)
				return;

			for (let iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++)
			{
				let animation: Animation.LerpAnimator = this.lerpAnimators[iIdx];

				animation.update (timeDelta);
			}

			if (this.onUpdate != null)
				this.onUpdate (timeDelta);
		}
	}

	export namespace Animation
	{
		export class Animator
		{
			public animation: Animation;
			public onAnimate: (attachedObject: HTMLElement | RadJav.GUI.GObject | RadJav.C3D.Object3D, data: any, timeDelta: number) => void;

			constructor (animation: Animation)
			{
				this.animation = animation;
				this.onAnimate = null;
			}

			animate (data: any, timeDelta: number): void
			{
				if (this.onAnimate != null)
					this.onAnimate (this.animation.attachedObject, data, timeDelta);
			}

			complete ()
			{
				if (this.animation.onCompleted != null)
					this.animation.onCompleted ();

				this.animation.stop ();
			}
		}

		export class LerpAnimator extends Animator
		{
			public startPos: RadJav.Vector3;
			public endPos: RadJav.Vector3;
			public startTime: number;
			public timeElapsed: number;
			public speed: number;

			constructor (animation: Animation, start: RadJav.Vector3, end: RadJav.Vector3, speed: number)
			{
				super (animation);

				this.startPos = start;
				this.endPos = end;
				this.speed = speed;
				this.startTime = 0;
				this.timeElapsed = 0;
			}

			update (timeDelta: number): void
			{
				let currentPos: RadJav.Vector3 = null;

				this.timeElapsed += timeDelta;

				let timeMoved: number = ((this.timeElapsed - this.startTime) / this.speed);

				currentPos = RadJav.Vector3.lerp (this.startPos, this.endPos, Math.clamp (timeMoved, 0, 1));

				this.animate (currentPos, timeDelta);

				if (currentPos.distance (this.endPos) == 0)
					this.complete ();
			}
		}

		export class PlayState
		{
			static Stopped: number = 1;
			static Playing: number = 2;
			static Paused: number = 3;
		}
	}
}