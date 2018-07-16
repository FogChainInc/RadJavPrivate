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
#include "cpp/RadJavCPPC3DModelFactory.h"

#include "RadJav.h"
#include <OgreColladaLoader.h>
#include <boost/filesystem.hpp>

namespace RadJAV
{
	namespace CPP
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			ModelFactory::ModelFactory()
			{
			}
			
			ModelFactory::~ModelFactory()
			{
			}
			
			Model* ModelFactory::load (Ogre::SceneManager* sceneManager,
									   const String& name,
									   const String& filePath,
									   Object3D *parent)
			{
				boost::filesystem::path resourcesPath(filePath);
				if(resourcesPath.empty() || !sceneManager)
					return nullptr;
				
				resourcesPath.remove_filename();

				Model* model = RJNEW Model( *sceneManager,
										    name,
										    filePath,
										    parent);
				
				
				
				try
				{
					Ogre::ColladaLoader::load (filePath, sceneManager, model->node, resourcesPath.string());
				}
				catch(...)
				{
					DELETEOBJ(model);
					model = nullptr;
				}
				
				return model;
			}
#endif
		}
	}
}
