#ifndef _OGRECOLLADALOADER_H_
#define _OGRECOLLADALOADER_H_

#include "OgreColladaLoaderExport.h"

#include <OgreString.h>

namespace Ogre
{
	class SceneManager;
	class SceneNode;
}

namespace Ogre {
	class OCL_EXPORT ColladaLoader
	{
	public:
		ColladaLoader() = delete;
		ColladaLoader(const ColladaLoader& other) = delete;

	public:
		static void load( const Ogre::String& daeFilePath,
						 Ogre::SceneManager* sceneManager,
						 Ogre::SceneNode* parentNode,
						 const Ogre::String& resourcesPath);

	};
}

#endif
