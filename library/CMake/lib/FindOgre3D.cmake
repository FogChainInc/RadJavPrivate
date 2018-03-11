message (STATUS "Searching for Ogre 3D...")

set (OGRE_SOURCE $ENV{OGRE_HOME} CACHE PATH "Ogre3D path")
fixPath (OGRE_SOURCE)
set (OGRE_SEARCH_PATHS "${OGRE_SOURCE}" ${RADJAV_DEPENDENCIES})

searchForHeader (OGRE Ogre.h "${OGRE_SEARCH_PATHS}" FALSE)
searchForHeader (OGRE OgreBuildSettings.h "${OGRE_SEARCH_PATHS} ${OGRE_SEARCH_PATHS}/build/include" FALSE)
searchForLibrary (OGRE OgreMain_d OgreMain ${OGRE_SEARCH_PATHS})

hasLibBeenFound (OGRE)

