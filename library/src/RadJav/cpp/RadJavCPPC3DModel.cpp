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
#include "cpp/RadJavCPPC3DModel.h"

#ifdef C3D_USE_OGRE

#include "RadJav.h"
#include "cpp/RadJavCPPC3DModelFactory.h"
#include <OgreColladaLoader.h>
#include <boost/filesystem.hpp>

namespace RadJAV
{
	namespace CPP
	{
		namespace C3D
		{
			Model::Model(const GUI::Canvas3D& canvas,
						 const String& filePath,
						 const String& name,
						 Object3D *parent)
			: Object3D(canvas, name, parent),
			  loaded(false),
			  filePath(filePath),
			  sceneManager(canvas.getSceneManager())
			{}

			bool Model::load()
			{
				if(loaded)
					return true;
				
				boost::filesystem::path resourcesPath(filePath);
				
				if(resourcesPath.empty() || !sceneManager)
					return false;
				
				resourcesPath.remove_filename();
				
				try
				{
					Ogre::ColladaLoader::load (filePath, sceneManager, node, resourcesPath.string());
				}
				catch(...)
				{
					return false;
				}
				
				loaded = true;
				
				return true;
			}
		}
	}
}
#endif
