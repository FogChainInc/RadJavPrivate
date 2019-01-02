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
#ifndef _RADJAV_GUI_CPP_LIGHT_H_
#define _RADJAV_GUI_CPP_LIGHT_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPC3DObject3D.h"

#ifdef C3D_USE_OGRE
#include <Ogre.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			/**
			 * @ingroup group_3d
			 * @brief 3D light class.
			 * @details Class representing light object in 3D scene.
			 */
			class RADJAV_EXPORT Light : public Object3D
			{
			public:
				Light(const GUI::Canvas3D& canvas,
					  Ogre::Light::LightTypes type,
					  const String& name,
					  Object3D *parent = NULL);

				void setType(Ogre::Light::LightTypes type);
				Ogre::Light::LightTypes getType() const;
				
				void setDiffuseColor(const Ogre::ColourValue& color);
				Ogre::ColourValue getDiffuseColor(void) const;
				
				Ogre::Light* light;
			};
#endif
		}
	}
}
#endif
