cmake_minimum_required(VERSION 3.11)

project (OgreColladaLoader)

message (STATUS "Configuring ${CMAKE_PROJECT_NAME}...")

# trying to find Ogre3D
# we can specify OGRE_HOME as a hint
set (OGRE_HOME "" CACHE PATH "Path to Ogre3D SDK install folder")

if ((DEFINED OGRE_HOME) AND (EXISTS ${OGRE_HOME}/CMake))
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${OGRE_HOME} ${OGRE_HOME}/CMake )
endif ()

if (APPLE)
	# help to find Ogre3D frameworks location (bug in FindOgre.cmake)
	set (OGRE_FRAMEWORK_PATH "" CACHE PATH "Path where Ogre3D Apple frameworks reside")
	set (CMAKE_FRAMEWORK_PATH ${OGRE_FRAMEWORK_PATH})
endif ()

find_package(OGRE 1.10.11 REQUIRED)

# trying to find Boost
# we can specify BOOST_ROOT as a hint
set (BOOST_ROOT "" CACHE PATH "Path to Boost install folder")
find_package(Boost REQUIRED chrono date_time system thread filesystem regex)

# trying to find OpenCOLLADA
# we can specify COLLADA_ROOT as a hint (location of installed OpenCOLLADA library)
set (COLLADA_ROOT "" CACHE PATH "Path to OpenCOLLADA install folder")

# headers
find_path(COLLADAFW_INCLUDE_DIR COLLADAFWRoot.h HINTS "${COLLADA_ROOT}/include/opencollada/COLLADAFramework")
find_path(COLLADASAX_INCLUDE_DIR COLLADASaxFWLLoader.h HINTS "${COLLADA_ROOT}/include/opencollada/COLLADASaxFrameworkLoader")
find_path(COLLADABASE_INCLUDE_DIR COLLADABUPlatform.h HINTS "${COLLADA_ROOT}/include/opencollada/COLLADABaseUtils")
find_path(COLLADASW_INCLUDE_DIR COLLADASWStreamWriter.h HINTS "${COLLADA_ROOT}/include/opencollada/COLLADAStreamWriter")
find_path(COLLADA_GENERATED_SAX_INCLUDE_DIR GeneratedSaxParser.h HINTS "${COLLADA_ROOT}/include/opencollada/GeneratedSaxParser")

# libraries
set( COLLADA_LIB_HINT "${COLLADA_ROOT}/lib/opencollada" )

find_library (COLLADABU_LIB OpenCOLLADABaseUtils HINTS ${COLLADA_LIB_HINT})
find_library (COLLADAFW_LIB OpenCOLLADAFramework HINTS ${COLLADA_LIB_HINT})
find_library (COLLADASAX_LIB OpenCOLLADASaxFrameworkLoader HINTS ${COLLADA_LIB_HINT})
find_library (COLLADASAXP_LIB GeneratedSaxParser HINTS ${COLLADA_LIB_HINT})
find_library (COLLADASW_LIB OpenCOLLADAStreamWriter HINTS ${COLLADA_LIB_HINT})
find_library (COLLADABF_LIB buffer HINTS ${COLLADA_LIB_HINT})
find_library (F2A_LIB ftoa HINTS ${COLLADA_LIB_HINT})
find_library (UTF_LIB UTF HINTS ${COLLADA_LIB_HINT})
find_library (MATHML_LIB MathMLSolver ${COLLADA_LIB_HINT})

# on Windows also get libxml2 and pcre from OpenCOLLADA
if (WIN32)
  find_library (XML2_LIB xml HINTS ${COLLADA_LIB_HINT})
  find_library (PCRE_LIB pcre HINTS ${COLLADA_LIB_HINT})
else()
  set (XML2_LIB xml2)
  set (PCRE_LIB pcre)
endif()

# trying to find OgreColladaImporter
# we can specify OGRECOLLADA_HEADERS_PATH and OGRECOLLADA_LIBRARIES_PATH
set (OGRECOLLADA_HEADERS_PATH "" CACHE PATH "Path to ColladaOgreImporter headers")
set (OGRECOLLADA_LIBRARIES_PATH "" CACHE PATH "Path to ColladaOgreImporter libraries")

# headers
find_path (OGRECOLLADA_INCLUDE_DIR OgreColladaSaxLoader.h HINTS ${OGRECOLLADA_HEADERS_PATH})
find_library (OGRECOLLADA_LIB collada_importer HINTS ${OGRECOLLADA_LIBRARIES_PATH})

# main project flags
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_VISIBILITY_PRESET hidden)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# add sources
set (HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/OgreColladaLoader.h")
set (SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreColladaLoader.cpp")

# add include dir
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories ("${PROJECT_BINARY_DIR}/include")
include_directories (SYSTEM ${OGRE_INCLUDE_DIRS})
include_directories (SYSTEM ${COLLADAFW_INCLUDE_DIR} ${COLLADASAX_INCLUDE_DIR} ${COLLADA_GENERATED_SAX_INCLUDE_DIR} ${COLLADABASE_INCLUDE_DIR})
include_directories (SYSTEM ${OGRECOLLADA_INCLUDE_DIR})

# make it as a shared library
add_library (ogrecolladaloader SHARED ${SOURCES} ${HEADERS})

# specify dependencies
target_link_libraries(ogrecolladaloader ${COLLADASAX_LIB}
					${COLLADASAXP_LIB}
					${COLLADAFW_LIB}
					${COLLADABU_LIB}
					${UTF_LIB}
					${XML2_LIB}
					${PCRE_LIB}
					${MATHML_LIB}
					${OGRECOLLADA_LIB}
					${OGRE_LIBRARIES}
					${Boost_LIBRARIES})

#if (WIN32)
  # libxml2 needs this
#  target_link_libraries(ogrecolladaloader Ws2_32 )
#endif()

# generating export header for library
include (GenerateExportHeader)
generate_export_header (ogrecolladaloader
			BASE_NAME OgreColladaLoader
			EXPORT_MACRO_NAME OCL_EXPORT
			NO_EXPORT_MACRO_NAME OCL_PRIVATE
			DEPRECATED_MACRO_NAME OCL_DEPRECATED
			NO_DEPRECATED_MACRO_NAME OCL_NO_DEPRECATED
			EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/OgreColladaLoaderExport.h")

install (TARGETS ogrecolladaloader DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/OgreColladaLoader.h"
		"${PROJECT_BINARY_DIR}/include/OgreColladaLoaderExport.h"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
