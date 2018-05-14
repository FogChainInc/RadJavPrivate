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
#include "cpp/RadJavCPPC3DCamera.h"

#include "RadJav.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			Camera::Camera( Ogre::SceneManager& sceneManager,
						   	Ogre::RenderWindow& renderWindow,
						    const String& name,
						   Object3D* parent)
			: Object3D(sceneManager, name, parent)
			{
				camera = sceneManager.createCamera(name);
				viewport = renderWindow.addViewport(camera);
				viewport->setClearEveryFrame(true);
				node->attachObject(camera);
			}

			Camera::~Camera()
			{
				node->detachObject(camera);
			}
			
			void Camera::setMode(Ogre::ProjectionType mode)
			{
				camera->setProjectionType(mode);
			}
			
			Ogre::ProjectionType Camera::getMode() const
			{
				return camera->getProjectionType();
			}
			
			void Camera::setAspectRatio(Ogre::Real aspectRatio)
			{
				camera->setAspectRatio(aspectRatio);
			}
			
			Ogre::Real Camera::getAspectRatio() const
			{
				return camera->getAspectRatio();
			}
			
			void Camera::setAutoAspectRatio(bool autoAspectRatio)
			{
				camera->setAutoAspectRatio(autoAspectRatio);
			}
			
			bool Camera::getAutoAspectRatio() const
			{
				return camera->getAutoAspectRatio();
			}

			void Camera::setFOV(const Ogre::Radian& fov)
			{
				camera->setFOVy(fov);
			}
			
			Ogre::Radian Camera::getFOV() const
			{
				return camera->getFOVy();
			}
			
			void Camera::setNearClipPlane(Ogre::Real nearDistance)
			{
				camera->setNearClipDistance(nearDistance);
			}
			
			Ogre::Real Camera::getNearClipPlane() const
			{
				return camera->getNearClipDistance();
			}
			
			void Camera::setFarClipPlane(Ogre::Real farDistance)
			{
				camera->setFarClipDistance(farDistance);
			}
			
			Ogre::Real Camera::getFarClipPlane() const
			{
				return camera->getFarClipDistance();
			}
			
			void Camera::setBackgroundColor(const Ogre::ColourValue& colour)
			{
				viewport->setBackgroundColour(colour);
			}
			
			const Ogre::ColourValue& Camera::getBackgroundColor() const
			{
				return viewport->getBackgroundColour();
			}
#endif
		}
	}
}

