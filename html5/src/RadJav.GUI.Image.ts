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
/// <reference path="RadJav.Vector2.ts" />

namespace RadJav
{
  export namespace GUI
  {
    /** 
     * An image.
     * Available on platforms: Windows,Linux,OSX,iOS,Android,HTML5
     */
    export class Image extends RadJav.GUI.GObject
    {
      static xmlTag: TagType = { tag: "image", type: "Image" };
      static ScaleModeAspectFit: number = 1;
      static ScaleModeAspectFill: number = 2;

      constructor(obj?: any, text?: string, parent?: RadJav.GUI.GObject)
      {
        if (obj == null) {
          obj = {};
        }

        if (typeof obj == "string") {
          var name = obj;
          obj = { name: name };
        }

        if (obj.size == null) {
          obj.size = new RadJav.Vector2();
          obj.size.x = 100;
          obj.size.y = 100;
        }

        super(obj, text, parent);

        this.type = "RadJav.GUI.Image";

        if (obj.image != null) {
          obj._image = obj.image;
        }
        this._image = RadJav.setDefaultValue(obj._image, null);
      }

      /** 
       * The image thats being used. If a string, it will be converted into
       * an Image when the image is set.
       */
      protected _image: any;

      
      public onCreated(): void {
        
        if (this._image != null) {
          this.setImage(this._image);
        }
      }

      /** 
       * Set the image.
       * Theme Event: setImage
       * Is Theme Event Asynchronous: Yes
       * @param {String/Image} image The image to display.
       * @return {Promise} Executes the promise when the image has loaded.
       */
      public setImage(image: string | any): void {
        this._image = image;

        if(this["_setImage"] != null)
				{
					this["_setImage"].apply(this, arguments);
					return;
				}

        RadJav.currentTheme.event(this.type, "setImage", this, image);
      }

      /** 
       * Set scale mode.
       */
      public setScaleMode(mode: number): void {
        if(this["_setScaleMode"] != null)
				{
					this["_setScaleMode"].apply(this, arguments);
				}
      }

      /** 
       * Get scale mode.
       */
      public getScaleMode(): number {
        if(this["_getScaleMode"] != null)
				{
					return this["_getScaleMode"].apply(this, arguments);
        }
        
        return Image.ScaleModeAspectFit;
      }
    }
  }
}

