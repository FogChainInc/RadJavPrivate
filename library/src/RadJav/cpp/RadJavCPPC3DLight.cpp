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
#include "cpp/RadJavCPPC3DLight.h"

#include "RadJav.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			Light::Light(const GUI::Canvas3D& canvas,
						 Ogre::Light::LightTypes type,
						 const String& name,
						 Object3D* parent)
			: Object3D(canvas, name, parent)
			{
				Ogre::SceneManager* sceneManager = canvas.getSceneManager();
				if(!sceneManager)
					return;
				
				light = sceneManager->createLight(name);
				light->setType(type);

				//User will use node system to set direction of the light afterwards
				light->setDirection(0, 0, -1);
				
				node->attachObject(light);
			}
			
			void Light::setType(Ogre::Light::LightTypes type)
			{
				if(light)
					light->setType(type);
			}
			
			Ogre::Light::LightTypes Light::getType() const
			{
				if(light)
					return light->getType();
				
				return Ogre::Light::LT_DIRECTIONAL;
			}
			
			void Light::setDiffuseColor(const Ogre::ColourValue& color)
			{
				if(light)
					light->setDiffuseColour(color);
			}
			
			Ogre::ColourValue Light::getDiffuseColor(void) const
			{
				if(light)
					return light->getDiffuseColour();
				
				return Ogre::ColourValue(0.0, 0.0, 0.0, 1.0);
			}
#endif
		}
	}
}
