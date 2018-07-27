#include "OgreColladaLoader.h"

#include <Ogre.h>

#include <OgreColladaSaxLoader.h>
#include <OgreSceneWriter.h>
#include <COLLADAFWRoot.h>

void Ogre::ColladaLoader::load( const Ogre::String& daeFilePath,
							   Ogre::SceneManager* sceneManager,
							   Ogre::SceneNode* parentNode,
							   const Ogre::String& resourcesPath)
{
	OgreCollada::SceneWriter writer(sceneManager,
									parentNode,
									resourcesPath);
	
	OgreCollada::SaxLoader loader;
	COLLADAFW::Root root(&loader, &writer);
	if (!root.loadDocument(daeFilePath))
	{
		OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Unable to load COLLADA file", "ColladaLoader::load");
	}
}

