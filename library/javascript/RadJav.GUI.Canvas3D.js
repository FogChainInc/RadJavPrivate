/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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

/** @class RadJav.GUI.Canvas3D
* @extends RadJav.GUI.GObject
* A 3d canvas.
* Available on platforms: Windows,Linux,OSX,HTML5
*/
RadJav.GUI.Canvas3D = (function (_super)
{
	__extends(Canvas3D, _super);

	function Canvas3D (obj, text, parent)
	{
		if (obj == null)
			obj = {};

		if (typeof (obj) == "string")
		{
			var name = obj;
			obj = { name: name };
		}

		if (obj.size == null)
		{
			obj.size = new RadJav.Vector2 ();
			obj.size.x = 500;
			obj.size.y = 350;
		}

		var _this = _super.call(this, obj, text, parent) || this;

		_this.type = "RadJav.GUI.Canvas3D";
		
		_this.children = [];

		/** @property {Mixed} [_renderer=null]
		* @protected
		* The renderer used to render the canvas.
		*/
		_this._renderer = RadJav.setDefaultValue (obj._renderer, null);
		/** @property {Number} [_rendererType=1]
		* @protected
		* The renderer type used to render the canvas.
		*/
		_this._rendererType = RadJav.setDefaultValue (obj._renderer, 1);
		/** @property {Object} [_currentCamera=null]
		* @protected
		* The current camera used to render the scene.
		*/
		_this._currentCamera = RadJav.setDefaultValue (obj._currentCamera, null);
		/** @property {Object} [_models={}]
		* @protected
		* The models that have been loaded for use. Each key is a RadJav.C3D.Model.
		*/
		_this._models = RadJav.setDefaultValue (obj._models, {});
		/** @property {Object} [_materials={}]
		* @protected
		* The materials that have been loaded for use. Each key is a RadJav.C3D.Material.
		*/
		_this._materials = RadJav.setDefaultValue (obj._materials, {});

        Canvas3D.prototype.setAmbientLightColor = function (color) {
            if (this._setAmbientLightColor != null) {
                this._setAmbientLightColor.apply(this, arguments);
            }
        };
        
        Canvas3D.prototype.getAmbientLightColor = function () {
            if (this._getAmbientLightColor != null) {
                return this._getAmbientLightColor();
            }
        };

        Canvas3D.prototype.addToScene = function (child) {
            this.children.push(child);
            if (this._addToScene != null) {
                this._addToScene(child);
            }
        };
        
        Canvas3D.prototype.removeFromScene = function (child) {
            this.children.slice(this.children.indexOf(child), 1);
            if (this._removeFromScene != null) {
                this._removeFromScene(child);
            }
        };
        
        Canvas3D.prototype.createObject3D = function (name) {
            if(this._createObject3D != null)
            {
                this._createObject3D.apply(this, arguments);
            }
        };
        
        Canvas3D.prototype.createCamera = function (name) {
            if(this._createCamera != null)
            {
                this._createCamera.apply(this, arguments);
            }
        };
        
        Canvas3D.prototype.createLight = function (name) {
            if(this._createLight != null)
            {
                this._createLight.apply(this, arguments);
            }
        };
        
        Canvas3D.prototype.createPlane = function (name) {
            if(this._createPlane != null)
            {
                this._createPlane.apply(this, arguments);
            }
        };
        
        Canvas3D.prototype.createCube = function (name) {
            if(this._createCube != null)
            {
                this._createCube.apply(this, arguments);
            }
        };
        
        Canvas3D.prototype.createSphere = function (name) {
            if(this._createSphere != null)
            {
                this._createSphere.apply(this, arguments);
            }
        };
        
        Canvas3D.prototype.loadModel = function (path, name) {
            if(this._loadModel != null)
            {
                return this._loadModel.apply(this, arguments);
            }
        };

		return (_this);
	}

	return (Canvas3D);
}(RadJav.GUI.GObject));

/** @class RadJav.GUI.Canvas3D.RendererTypes
* A 3d canvas.
* Available on platforms: Windows,Linux,OSX,HTML5
*/
RadJav.GUI.Canvas3D.RendererTypes = function ()
{
}

RadJav.GUI.Canvas3D.RendererTypes.AnyAvailable = 1;
RadJav.GUI.Canvas3D.RendererTypes.WebGL = 2;
RadJav.GUI.Canvas3D.RendererTypes.Context2D = 3;

