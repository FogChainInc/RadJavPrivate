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

			Transform::Transform( Ogre::SceneManager& sceneManager, const String& name, Transform *parent)
			: sceneMgr(sceneManager)
			{
				node = sceneManager.createSceneNode();
				
				if(parent == NULL)
				{
					sceneManager.getRootSceneNode()->addChild(node);
				}
				else
				{
					parent->node->addChild(node);
				}
			}
			
			Transform::~Transform()
			{
				node->removeAndDestroyAllChildren();
				sceneMgr.destroySceneNode(node);
			}
			
			String Transform::getName() const
			{
				return node->getName();
			}
			
			const Ogre::Vector3& Transform::getPosition() const
			{
				return node->getPosition();
			}
			
			const Ogre::Vector3& Transform::getScale() const
			{
				return node->getScale();
			}
			
			void Transform::addChild(Transform* child)
			{
				if(!child)
					return;
				
				Ogre::SceneNode* parent = child->node->getParentSceneNode();
				if(parent)
				{
					parent->removeChild(child->node);
				}
				
				node->addChild(child->node);
			}
			
			Transform* Transform::removeChild(Transform* child)
			{
				if(!child)
					return child;
				
				node->removeChild(child->node);
				
				return child;
			}
			
			void Transform::removeAndDestroyChild(Transform* child)
			{
				if(!child)
					return;
				
				removeChild(child);
				delete child;
			}
			
			void Transform::pitch(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo)
			{
				node->pitch(angle, relativeTo);
			}
			
			void Transform::roll(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo)
			{
				node->roll(angle, relativeTo);
			}
			
			void Transform::yaw(const Ogre::Radian &angle, Ogre::Node::TransformSpace relativeTo)
			{
				node->yaw(angle, relativeTo);
			}
			
			void Transform::setOrientation(const Ogre::Quaternion& orientation)
			{
				node->setOrientation(orientation);
			}
			
			void Transform::setPosition(const Ogre::Vector3& position)
			{
				node->setPosition(position);
			}
			
			void Transform::setScale(const Ogre::Vector3& scale)
			{
				node->setScale(scale);
			}
			
			void Transform::lookAt(const Ogre::Vector3& targetPoint, Ogre::Node::TransformSpace relativeTo, const Ogre::Vector3& localDirectionVector)
			{
				node->lookAt(targetPoint, relativeTo, localDirectionVector);
			}
			
			void Transform::translate(const Ogre::Vector3& direction, Ogre::Node::TransformSpace relativeTo)
			{
				node->translate(direction, relativeTo);
			}
			
			void Transform::lookAt(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Node::TransformSpace relativeTo)
			{
				node->lookAt( Ogre::Vector3(x, y, z), relativeTo);
			}
			
			void Transform::lookAt(const Ogre::Vector3& point, Ogre::Node::TransformSpace relativeTo)
			{
				node->lookAt(point, relativeTo);
			}

			#endif
		}
	}
}
