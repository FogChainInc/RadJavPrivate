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
	/**
	 * @brief Class which handle import of \b COLLADA files.
	 */
	class OCL_EXPORT ColladaLoader
	{
	public:
		ColladaLoader() = delete;
		ColladaLoader(const ColladaLoader& other) = delete;

	public:
		/**
		 * @brief Method to load \b COLLADA object into scene.
		 * @details Use this method to load \b COLLADA objects from files and attach them into scene node.
		 * @param[in] daeFilePath Absolute path to COLLADA(\c .dae) file.
		 * @param[in] sceneManager Pointer to current scene manager which manage 3D objects.
		 * @param[in] parentNode Pointer to parent node to which loaded 3D object will be attached.
		 * @param[in] resourcesPath Absolute path to folder where additional resources reside, for example textures used in 3D model.
		 * @exception ERR_INTERNAL_ERROR If error occur during file loading process.
		 */
		static void load( const Ogre::String& daeFilePath,
						 Ogre::SceneManager* sceneManager,
						 Ogre::SceneNode* parentNode,
						 const Ogre::String& resourcesPath);

	};
}

#endif
