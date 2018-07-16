message (STATUS "Searching for TinyXML2...")

set (TinyXML2_SOURCE $ENV{TinyXML2_SOURCE} CACHE PATH "TinyXML2 path")
fixPath (TinyXML2_SOURCE)

set (TinyXML2_SEARCH_PATHS ${TinyXML2_SOURCE} ${RADJAV_DEPENDENCIES})

if (TinyXML2_SEARCH_PATHS)
	searchForHeader (TinyXML2 tinyxml2.h ${TinyXML2_SEARCH_PATHS} FALSE)
	
	set (OTHER_SEARCH_DIRS ${TinyXML2_SEARCH_PATHS}/tinyxml2 ${TinyXML2_SEARCH_PATHS}/tinyxml2/Release-Dll)

	searchForLibraryOnlyInPath (TinyXML2 tinyxml2 tinyxml2 ${TinyXML2_SEARCH_PATHS} ${OTHER_SEARCH_DIRS} ${OTHER_SEARCH_DIRS})
endif ()

hasLibBeenFound (TinyXML2)

