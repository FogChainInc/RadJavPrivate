message (STATUS "Searching for Ogre 3D...")

set (OGRE_SOURCE $ENV{OGRE_HOME} CACHE PATH "Ogre3D path")
fixPath (OGRE_SOURCE)
set (OGRE_SEARCH_PATHS "${OGRE_SOURCE}" ${RADJAV_DEPENDENCIES})

if (APPLE)
	set (OGRE_SEARCH_PATHS "${OGRE_SOURCE}/lib/macosx" "${OGRE_SOURCE}/lib/debug" "${OGRE_SOURCE}/build/lib/macosx" ${OGRE_SEARCH_PATHS})
endif ()

searchForHeader (OGRE Ogre.h "${OGRE_SEARCH_PATHS}" FALSE)
searchForHeader2 (OGRE OgreBuildSettings.h "${OGRE_SEARCH_PATHS}" FALSE)

if (APPLE)
	searchForLibrary (OGRE Ogre Ogre "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgreBites OgreBites "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgreHLMS OgreHLMS "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgreMeshLodGenerator OgreMeshLodGenerator "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgreOverlay OgreOverlay "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgrePaging OgrePaging "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgreProperty OgreProperty "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgreRTShaderSystem OgreRTShaderSystem "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgreTerrain OgreTerrain "${OGRE_SEARCH_PATHS}")
	searchForLibrary (OGRE OgreVolume OgreVolume "${OGRE_SEARCH_PATHS}")
else ()
	searchForLibrary (OGRE OgreMain_d OgreMain ${OGRE_SEARCH_PATHS})
endif ()

hasLibBeenFound (OGRE)

