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
#include "cpp/RadJavCPPC3DTransform.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace C3D
		{

		#ifdef C3D_USE_OGRE

			Transform::Transform(const GUI::Canvas3D& canvas,
								 const String& name,
								 Transform *parent)
			{
				sceneManager = canvas.getSceneManager();
				
				if(!sceneManager)
					return;
				
				node = sceneManager->createSceneNode();
				
				if(parent)
				{
					parent->addChild(this);
				}
			}
			
			Transform::~Transform()
			{
				//TODO: think about memory leaks
				
				//node->removeAndDestroyAllChildren();
				sceneManager->destroySceneNode(node);
			}
			
			String Transform::getName() const
			{
				if(node)
					return node->getName();
				
				return String();
			}
			
			Ogre::Vector3 Transform::getPosition() const
			{
				if(node)
					return node->getPosition();
				
				return Ogre::Vector3(0.0, 0.0, 0.0);
			}
			
			Ogre::Vector3 Transform::getScale() const
			{
				if(node)
					return node->getScale();
				
				return Ogre::Vector3(1.0, 1.0, 1.0);
			}
			
			void Transform::addChild(Transform* child)
			{
				if(!child)
					return;
				
				if(node && child->node)
				{
					Ogre::SceneNode* parent = child->node->getParentSceneNode();
					if(parent)
					{
						parent->removeChild(child->node);
					}
					
					node->addChild(child->node);
				}
			}
			
			Transform* Transform::removeChild(Transform* child)
			{
				if(!child)
					return child;
				
				if(node && child->node)
					node->removeChild(child->node);
				
				return child;
			}
			
			void Transform::removeAndDestroyChild(Transform* child)
			{
				if(!child)
					return;
				
				removeChild(child);
				
				//TODO: think about memory leaks
				delete child;
			}
			
			void Transform::pitch(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo)
			{
				if(node)
					node->pitch(angle, relativeTo);
			}
			
			void Transform::roll(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo)
			{
				if(node)
					node->roll(angle, relativeTo);
			}
			
			void Transform::yaw(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo)
			{
				if(node)
					node->yaw(angle, relativeTo);
			}
			
			void Transform::setDirection(const Ogre::Vector3& direction,
										 Ogre::Node::TransformSpace relativeTo,
										 const Ogre::Vector3& localDirectionVector)
			{
				if(node)
					node->setDirection(direction, relativeTo, localDirectionVector);
			}

			void Transform::setDirection(Ogre::Real x, Ogre::Real y, Ogre::Real z,
										 Ogre::Node::TransformSpace relativeTo,
										 const Ogre::Vector3& localDirectionVector)
			{
				if(node)
					node->setDirection(x, y, z, relativeTo, localDirectionVector);
			}
			
			void Transform::setOrientation(const Ogre::Quaternion& orientation)
			{
				if(node)
					node->setOrientation(orientation);
			}
			
			void Transform::setPosition(const Ogre::Vector3& position)
			{
				if(node)
					node->setPosition(position);
			}
			
			void Transform::setScale(const Ogre::Vector3& scale)
			{
				if(node)
					node->setScale(scale);
			}
			
			void Transform::lookAt(const Ogre::Vector3& targetPoint, Ogre::Node::TransformSpace relativeTo, const Ogre::Vector3& localDirectionVector)
			{
				if(node)
					node->lookAt(targetPoint, relativeTo, localDirectionVector);
			}
			
			void Transform::translate(const Ogre::Vector3& direction, Ogre::Node::TransformSpace relativeTo)
			{
				if(node)
					node->translate(direction, relativeTo);
			}
			
			void Transform::lookAt(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Node::TransformSpace relativeTo)
			{
				if(node)
					node->lookAt( Ogre::Vector3(x, y, z), relativeTo);
			}
			
			void Transform::lookAt(const Ogre::Vector3& point, Ogre::Node::TransformSpace relativeTo)
			{
				if(node)
					node->lookAt(point, relativeTo);
			}

			#endif
		}
	}
}
