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
#ifndef _RADJAV_GUI_CPP_TRANSFORM_H_
#define _RADJAV_GUI_CPP_TRANSFORM_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#ifdef C3D_USE_OGRE
#include <Ogre.h>
#include "cpp/RadJavCPPGUICanvas3D.h"
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
			 * @brief 3D transform class.
			 * @details Base class for all moveable objects in 3D scene.
			 */
			class RADJAV_EXPORT Transform
			{
			public:
				Transform(const GUI::Canvas3D& canvas,
						  const String& name,
						  Transform *parent = NULL);

				virtual ~Transform();
				
				String getName() const;
				
				Ogre::Vector3 getPosition() const;
				Ogre::Vector3 getScale() const;
				
				void addChild(Transform* child);
				Transform* removeChild(Transform* child);
				void removeAndDestroyChild(Transform* child);

				void pitch(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL);
				void roll(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL);
				void yaw(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL);
				
				void setDirection(const Ogre::Vector3& direction,
								  Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL,
								  const Ogre::Vector3& localDirectionVector = Ogre::Vector3::NEGATIVE_UNIT_Z);
				
				void setDirection(Ogre::Real x, Ogre::Real y, Ogre::Real z,
								  Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL,
								  const Ogre::Vector3& localDirectionVector = Ogre::Vector3::NEGATIVE_UNIT_Z);
				
				void setOrientation(const Ogre::Quaternion& orientation);
				void setPosition(const Ogre::Vector3& position);
				void setScale(const Ogre::Vector3& scale);
				void lookAt(const Ogre::Vector3& targetPoint, Ogre::Node::TransformSpace relativeTo, const Ogre::Vector3& localDirectionVector=Ogre::Vector3::NEGATIVE_UNIT_Z);
				void translate(const Ogre::Vector3& direction, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT);
				void lookAt(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
				void lookAt(const Ogre::Vector3& point, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);

				Ogre::SceneNode* node;
				Ogre::SceneManager* sceneManager;
			};
#endif
		}
	}
}
#endif
