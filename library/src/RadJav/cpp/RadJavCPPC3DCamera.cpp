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
			Camera::Camera( const GUI::Canvas3D& canvas,
						    const String& name,
						   	Object3D* parent)
			: Object3D(canvas, name, parent)
			{
				Ogre::SceneManager* sceneManager = canvas.getSceneManager();
				Ogre::RenderWindow* renderWindow = canvas.getRenderWindow();

				if(!sceneManager || !renderWindow)
					return;
				
				camera = sceneManager->createCamera(name);
				node->attachObject(camera);
				viewport = renderWindow->addViewport(camera);
				viewport->setClearEveryFrame(true);
			}

			void Camera::setPerspective(bool perspective)
			{
				if(camera)
					camera->setProjectionType(perspective ? Ogre::PT_PERSPECTIVE : Ogre::PT_ORTHOGRAPHIC);
			}
			
			bool Camera::isPerspective() const
			{
				if(camera)
					return camera->getProjectionType() == Ogre::PT_PERSPECTIVE;
				
				return true;
			}
			
			void Camera::setAspectRatio(Ogre::Real aspectRatio)
			{
				if(camera)
					camera->setAspectRatio(aspectRatio);
			}
			
			Ogre::Real Camera::getAspectRatio() const
			{
				if(camera)
					return camera->getAspectRatio();
				
				return 1.0;
			}
			
			void Camera::setAutoAspectRatio(bool autoAspectRatio)
			{
				if(camera)
					camera->setAutoAspectRatio(autoAspectRatio);
			}
			
			bool Camera::getAutoAspectRatio() const
			{
				if(camera)
					return camera->getAutoAspectRatio();
				
				return false;
			}

			void Camera::setFOV(const Ogre::Radian& fov)
			{
				if(camera)
					camera->setFOVy(fov);
			}
			
			Ogre::Radian Camera::getFOV() const
			{
				if(camera)
					return camera->getFOVy();
				
				return Ogre::Degree(60.0);
			}
			
			void Camera::setNearClipPlane(Ogre::Real nearDistance)
			{
				if(camera)
					camera->setNearClipDistance(nearDistance);
			}
			
			Ogre::Real Camera::getNearClipPlane() const
			{
				if(camera)
					return camera->getNearClipDistance();
				
				return 1.0;
			}
			
			void Camera::setFarClipPlane(Ogre::Real farDistance)
			{
				if(camera)
					camera->setFarClipDistance(farDistance);
			}
			
			Ogre::Real Camera::getFarClipPlane() const
			{
				if(camera)
					return camera->getFarClipDistance();
				
				return 100.0;
			}
			
			void Camera::setBackgroundColor(const Ogre::ColourValue& colour)
			{
				if(viewport)
					viewport->setBackgroundColour(colour);
			}
			
			Ogre::ColourValue Camera::getBackgroundColor() const
			{
				if(viewport)
					return viewport->getBackgroundColour();
				
				return Ogre::ColourValue(0.0, 0.0, 0.0, 1.0);
			}
#endif
		}
	}
}

