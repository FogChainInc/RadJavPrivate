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

namespace RadJav
{
	export namespace C3D
	{
		/** @class RadJav.C3D.World
		* A 3D world.
		* Available on platforms: Windows,Linux,OSX,HTML5
		*/
		export class World
		{
			constructor(obj?)
			{
				if (obj == null)
					obj = {};

				this._name = RadJav.setDefaultValue(obj._name, "defaultWorld");
				this._ambientLightColor = RadJav.setDefaultValue(obj._ambientLightColor, RadJav.Color.White);
				this._sceneManager = RadJav.setDefaultValue(obj._sceneManager, null);
				this._renderWindow = RadJav.setDefaultValue(obj._renderWindow, null);
			}

			/** @property {string} [_name="defaultWorld"]
			* @protected
			* The name of the world.
			*/
			_name: string;
			/** @property {RadJav.Color} [_ambientLightColor=RadJav.Color.White]
			* @protected
			* The ambient light color.
			*/
			_ambientLightColor: Color;
			/** @property {any} [_sceneManager=null]
			* @protected
			* The scene manager being used for this world.
			*/
			_sceneManager: any;
			/** @property {any} [_renderWindow=null]
			* @protected
			* The window being rendered to.
			*/
			_renderWindow: any;

			/** @method setAmbientLightColor
			* Set the position of this object.
			* @param {Number/RadJav.Vector3} x The x position or full vector3 position.
			* @param {Number} y The y position.
			* @param {Number} z The z position.
			*/
			setAmbientLightColor(color: Color): void
			{
				this._ambientLightColor = color;
			}

			/** @method getPosition
			* Get the position of this object.
			* @return {RadJav.Vector3} The position.
			*/
			getAmbientLightColor(): Color
			{
				return (this._ambientLightColor);
			}
		}
	}
}
