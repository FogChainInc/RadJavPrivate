message (STATUS "Searching for Collada Loader...")

set (COLLADA_SOURCE $ENV{COLLADALOADER_HOME} CACHE PATH "Path to OgreColladaLoader library install directory")
fixPath (COLLADA_SOURCE)
set (COLLADA_SEARCH_PATHS ${COLLADA_SOURCE} ${RADJAV_DEPENDENCIES})

searchForHeader (COLLADA OgreColladaLoader.h "${COLLADA_SEARCH_PATHS}" FALSE)

if (APPLE)
	searchForLibrary (COLLADA ogrecolladaloader ogrecolladaloader "${COLLADA_SEARCH_PATHS}")
else ()
	searchForLibrary (COLLADA ogrecolladaloader ogrecolladaloader "${COLLADA_SEARCH_PATHS}")
endif ()

hasLibBeenFound (COLLADA)
