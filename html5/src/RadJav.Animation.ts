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
		public lerpAnimators: LerpAnimator[];
		public onUpdate: (timeDelta: number) => void;
		public playState: PlayState;

		constructor ()
		{
			this.name = "";
			this.attachedObject = null;
			this.lerpAnimators = [];
			this.onUpdate = null;
			this.playState = PlayState.Stopped;
			RadJav.addAnimation (this);
		}

		attach (obj: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D): void
		{
			this.attachedObject = obj;
		}

		_getDisplayObject (obj: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D)
								: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D
		{
			let foundObj: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D = null;

			if (this.attachedObject.constructor["name"] == "HTMLElement")
				foundObj = (<HTMLElement>this.attachedObject);

			if (this.attachedObject.constructor["name"] == "Component")
				foundObj = this._getDisplayObject (this.attachedObject.display);

			if (this.attachedObject.constructor["name"] == "GObject")
				foundObj = (<RadJav.GUI.GObject>this.attachedObject);

			if (this.attachedObject.constructor["name"] == "Object3D")
				foundObj = (<RadJav.C3D.Object3D>this.attachedObject);

			return (foundObj);
		}

		lerp (speed: number, x: number | RadJav.Vector2 | RadJav.Vector3, y: number = 0, z: number = 0): void
		{
			let start: RadJav.Vector3 = null;
			let end: RadJav.Vector3 = new RadJav.Vector3 ();
			let foundObj: HTMLElement | RadJav.Interact.Component | RadJav.GUI.GObject | RadJav.C3D.Object3D = this._getDisplayObject (this.attachedObject);

			if (foundObj.constructor["name"] == "HTMLElement")
				start = new RadJav.Vector3 ((<HTMLElement>this.attachedObject).offsetLeft, (<HTMLElement>this.attachedObject).offsetTop, 0);

			if (foundObj.constructor["name"] == "GObject")
			{
				let pos: RadJav.Vector2 = (<RadJav.GUI.GObject>this.attachedObject.display).getPosition ();
				start = new RadJav.Vector3 (pos);
			}

			if (foundObj.constructor["name"] == "Object3D")
				start = (<RadJav.C3D.Object3D>this.attachedObject.display).getPosition ();

			if (typeof (x) == "number")
			{
				end.x = x;
				end.y = y;
				end.z = z;
			}
			else
			{
				if (x.constructor["name"] == "Vector2")
					end = new RadJav.Vector3 ((<RadJav.Vector2>x));

				if (x.constructor["name"] == "Vector3")
					end = new RadJav.Vector3 ((<RadJav.Vector3>x));
			}

			let animator = new LerpAnimator (foundObj, start, end, speed);
			animator.startTime = RadJav.getTime ();
			animator.onAnimate = function (animObj, newPos, timeDelta)
				{
					if (animObj.constructor["name"] == "HTMLElement")
					{
						animObj.offsetLeft = newPos.x;
						animObj.offsetTop = newPos.y;
					}
					else
						animObj.setPosition (newPos);
				};
			this.lerpAnimators.push (animator);
		}

		play (): void
		{
			for (let iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++)
			{
				let animation: LerpAnimator = this.lerpAnimators[iIdx];

				animation.startTime = RadJav.getTime ();
			}

			this.playState = PlayState.Playing;
		}

		pause (): void
		{
			for (let iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++)
			{
				let animation: LerpAnimator = this.lerpAnimators[iIdx];
			}

			this.playState = PlayState.Paused;
		}

		stop (): void
		{
			for (let iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++)
			{
				let animation: LerpAnimator = this.lerpAnimators[iIdx];
			}

			this.playState = PlayState.Stopped;
		}

		on (event: string, func: (timeDelta: number) => void)
		{
			if (event == "update")
				this.onUpdate = func;
		}

		update (timeDelta: number): void
		{
			if (this.playState != PlayState.Playing)
				return;

			for (let iIdx = 0; iIdx < this.lerpAnimators.length; iIdx++)
			{
				let animation: LerpAnimator = this.lerpAnimators[iIdx];

				animation.update (timeDelta);
			}

			if (this.onUpdate != null)
				this.onUpdate (timeDelta);
		}

		export class Animator
		{
			public attachedObject: HTMLElement | RadJav.GUI.GObject | RadJav.C3D.Object3D;
			public onAnimate: (attachedObject: HTMLElement | RadJav.GUI.GObject | RadJav.C3D.Object3D, data: any, timeDelta: number) => void;

			constructor (attachedObject: HTMLElement | RadJav.GUI.GObject | RadJav.C3D.Object3D)
			{
				this.attachedObject = attachedObject;
				this.onAnimate = null;
			}

			animate (data: any, timeDelta: number): void
			{
				if (this.onAnimate != null)
					this.onAnimate (this.attachedObject, data, timeDelta);
			}
		}

		export class LerpAnimator extends Animator
		{
			public startPos: RadJav.Vector3;
			public endPos: RadJav.Vector3;
			public startTime: number;
			public currentTime: number;
			public length: number;
			public speed: number;

			constructor (attachedObject: HTMLElement | RadJav.GUI.GObject | RadJav.C3D.Object3D, 
							start: RadJav.Vector3, end: RadJav.Vector3, speed: number)
			{
				super (attachedObject);

				this.startPos = start;
				this.endPos = end;
				this.speed = speed;
				this.startTime = 0;
				this.currentTime = 0;
				this.length = (end - start);
			}

			update (timeDelta: number): void
			{
				let currentPos: RadJav.Vector3 = new RadJav.Vector3 ();
				let posMoved: number = ((RadJav.getTime () - this.startTime) * this.speed);
				let posDelta: number = posMoved / this.length;

				currentPos = currentPos.lerp (this.startPos, this.endPos, posDelta);

				this.animate (currentPos, timeDelta);
			}
		}
	}

	export namespace Animation
	{
		export enum PlayState
		{
			Stopped, 
			Playing, 
			Paused
		}
	}
}