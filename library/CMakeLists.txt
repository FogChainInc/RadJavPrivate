cmake_minimum_required (VERSION 3.0)

if (WIN32)
	set (CMAKE_USE_RELATIVE_PATHS TRUE)
endif ()

project (libRadJav)

message (STATUS "Configuring libRadJav...")

set (CMAKE_MODULE_PATH "${libRadJav_SOURCE_DIR}/CMake"
			"${libRadJav_SOURCE_DIR}/CMake/lib"
			"${libRadJav_SOURCE_DIR}/include"
			"${libRadJav_SOURCE_DIR}/src")

include (Utils)

#Setting supported Javascript engines for specific platforms
if (APPLE)
	list (APPEND javascriptEngineLibraries JavaScriptCore V8)
	list (APPEND guiLibraries wxWidgets "iOS-UIKit")
elseif (WIN32 OR UNIX)
	list (APPEND javascriptEngineLibraries V8)
	list (APPEND guiLibraries wxWidgets Android)
endif ()

set (libRadJav_JS_LIBRARY V8 CACHE STRING "Javascript library to use")
set_property (CACHE libRadJav_JS_LIBRARY PROPERTY STRINGS ${javascriptEngineLibraries})

set (libRadJav_GUI_LIBRARY wxWidgets CACHE STRING "GUI library to use")
set_property (CACHE libRadJav_GUI_LIBRARY PROPERTY STRINGS ${guiLibraries})

set (libRadJav_HTTP_LIBRARY "curl" CACHE STRING "HTTP library to use")
set (libRadJav_INCLUDE_C3D_API TRUE CACHE BOOL "Include the C3D API?")
set (libRadJav_ALLOW_NETWORKING TRUE CACHE BOOL "Allow Networking?")
set (libRadJav_DEPENDENCIES $ENV{RADJAV_DEPENDENCIES} CACHE PATH "RadJav Dependencies path")
set (libRadJav_LANGUAGE "en_us" CACHE STRING "Select language to compile with")
set (libRadJav_STATIC false CACHE BOOL "Static build of RadJav")
set (libRadJav_EMBEDFILE true CACHE BOOL "Embed Javascript into RadJav?")
set (libRadJav_EMBEDFILE_CMAKE ON CACHE BOOL "Embed Javascript with CMAKE?")
set (libRadJav_INCLUDE_BLOCKCHAIN_V1 true CACHE BOOL "Include Blockchain V1?")
set (FIND_MEMORY_LEAKS true CACHE BOOL "Find memory leaks?")
set (INCLUDE_DATABASES true CACHE BOOL "Includes databases")
set (XML_SUPPORT true CACHE BOOL "XML Support")
set (INCLUDE_CRYPTOGRAPHY true CACHE BOOL "Includes cryptography")
set (WEBVIEW_TYPE "wxWidgetsDefault" CACHE STRING "Type of webview to use")
set (BOOST_ROOT "" CACHE PATH "BOOST_ROOT")

set (TEMP_ARCH "x86")
set (IS_MOBILE false)

if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
	set (TEMP_ARCH "x86_64")
endif()

if (NOT libRadJav_GUI_LIBRARY STREQUAL "wxWidgets")
	set (IS_MOBILE true)
endif ()

set (archs x86 x86_64 armeabi armeabi-v7a arm64-v8a)
set (USE_ARCH ${TEMP_ARCH} CACHE STRING "Processor architecture")
set_property (CACHE USE_ARCH PROPERTY STRINGS ${archs})

set (CMAKE_BUILD_TYPE CACHE STRING "Debug")
set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

set (libRadJav_LANGUAGE_UPPER)
string (TOUPPER ${libRadJav_LANGUAGE} libRadJav_LANGUAGE_UPPER)
message (STATUS "Using language ${libRadJav_LANGUAGE_UPPER}")

if (libRadJav_INCLUDE_C3D_API)
	message (STATUS "C3D API is going to be included in this build.")
	set (USE_OGRE TRUE)
else ()
	message (STATUS "C3D API will NOT be included in this build.")
endif ()

if (libRadJav_ALLOW_NETWORKING)
	message (STATUS "Networking is going to be included in this build.")
else ()
	message (STATUS "Networking will NOT be included in this build.")
endif ()

if (libRadJav_INCLUDE_BLOCKCHAIN_V1)
	message (STATUS "Blockchain V1 will be included in this build.")

	set (OPENSSL_ROOT_DIR "" CACHE PATH "OPENSSL_ROOT_DIR")
	set (ZLIB_ROOT "" CACHE PATH "ZLIB_ROOT")
else ()
	message (STATUS "Blockchain V1 will NOT be included in this build.")
endif ()

if (libRadJav_DEPENDENCIES)
	fixPath (libRadJav_DEPENDENCIES)
	message (STATUS "RadJav Dependencies path set to: ${libRadJav_DEPENDENCIES}")
endif ()

if (libRadJav_JS_LIBRARY STREQUAL "V8")
	set (USE_V8 true)
	set (libRadJav_DEBUG_INSPECTOR true CACHE BOOL "Include V8 js inspector?")
endif ()

if (libRadJav_JS_LIBRARY STREQUAL "JavaScriptCore")
	set (USE_JAVASCRIPTCORE true)
endif ()

if (libRadJav_GUI_LIBRARY STREQUAL "wxWidgets")
	set (USE_WXWIDGETS true)
endif ()

if (libRadJav_HTTP_LIBRARY STREQUAL "civetweb")
	set (USE_CIVETWEB true)
endif ()

if (libRadJav_HTTP_LIBRARY STREQUAL "nghttp2")
	set (USE_NGHTTP2 true)
endif ()

if (INCLUDE_DATABASES)
	set (databases LevelDB RocksDB NuDB "RocksDB and NuDB")
	set (libRadJav_DATABASE "RocksDB and NuDB" CACHE STRING "Database to link with")
	set_property (CACHE libRadJav_DATABASE PROPERTY STRINGS ${databases})
endif ()

set (CMAKE_DEBUG_POSTFIX "_d")
#set (libRadJav_SOURCE_LANGUAGE_FILE)

#set (SOURCES_files_Languages
#	"${libRadJav_SOURCE_DIR}/include/RadJav/languages/${libRadJav_LANGUAGE}.h")
#source_group ("Languages" FILES ${SOURCES_files_Languages})
set (SOURCES "${libRadJav_SOURCE_DIR}/src/RadJav" ${SOURCES})

#set (SOURCES ${SRC} ${SOURCES})

set (libRadJav_CPP_DEFS "LANG_${libRadJav_LANGUAGE_UPPER}" ${libRadJav_CPP_DEFS})
set (libRadJav_CPP_DEFS_DEBUG RADJAV_DEBUG ${libRadJav_CPP_DEFS_DEBUG})
set (libRadJav_CXX_FLAGS "")

include (FindModuleCurl)

if (USE_OGRE)
	include (FindOgre3D)

	set (libRadJav_USE_OGRE_COLLADA TRUE CACHE BOOL "Use Ogre Collada?")
	include (FindColladaLoader)
endif ()

if (USE_V8)
	include (FindV8)
endif ()

if (USE_JAVASCRIPTCORE)
	#include (FindJavaScriptCore)
	set (JAVASCRIPTCORE_FOUND true)
endif ()

if (USE_WXWIDGETS)
	include (FindModulewxWidgets)
endif ()

if (USE_NGHTTP2)
	include (FindModuleNGHTTP2)
endif ()

if (USE_CIVETWEB)
	include (FindModuleCivetWeb)
endif ()

if (XML_SUPPORT)
	include (FindTinyXML2)
endif ()

if (libRadJav_STATIC)
	set (libRadJav_CPP_DEFS RADJAV_LIB_STATIC ${libRadJav_CPP_DEFS})
else ()
	set (libRadJav_CPP_DEFS RADJAV_LIB_DLL ${libRadJav_CPP_DEFS})
endif ()

if (IS_MOBILE)
	if (libRadJav_GUI_LIBRARY STREQUAL "iOS-UIKit")
		set (libRadJav_CPP_DEFS USE_IOS ${libRadJav_CPP_DEFS})
		set (CMAKE_CXX_FLAGS "-x objective-c++" ${CMAKE_CXX_FLAGS})
	endif ()

	if (libRadJav_GUI_LIBRARY STREQUAL "Android")
		set (libRadJav_CPP_DEFS USE_ANDROID ${libRadJav_CPP_DEFS})
	endif ()
endif ()

if (WIN32)
	set (libRadJav_CPP_DEFS WIN32 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS _WINDOWS ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS ARCH_X86 ${libRadJav_CPP_DEFS})

	if (MSVC)
		set (libRadJav_CPP_DEFS NOMINMAX ${libRadJav_CPP_DEFS})

		if (MSVC_VERSION LESS 1910)
			message (FATAL_ERROR "You must use Visual Studio 2017 or better!")
		endif ()

		#find_program (NUGET nuget)

		#if (NUGET)
			#execute_process (COMMAND ${NUGET} install leveldb-vc140 libzmq-vc141 secp256k1-vc141)
		#else ()
			#message (WARNING "Cannot find nuget, some libraries will have to be installed manually.")
		#endif ()
	endif ()

	set (libRadJav_CPP_DEFS STDC99 ${libRadJav_CPP_DEFS})
endif ()

if (UNIX)
	set (libRadJav_CPP_DEFS LINUX ${libRadJav_CPP_DEFS})
endif ()

set (libRadJav_INCLUDE_DIRS)
set (libRadJav_LINK_LIBRARIES)

set (libRadJav_INCLUDE_DIRS ${libRadJav_SOURCE_DIR}/include/RadJav ${libRadJav_INCLUDE_DIRS})

# Boost
if ((${USE_ARCH} MATCHES "armeabi") OR (${USE_ARCH} MATCHES "armeabi-v7a") OR (${USE_ARCH} MATCHES "arm64-v8a"))
	SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/.../boost")
	SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/.../boost/lib")
endif()
include (FindModuleBoost)

set (libRadJav_INCLUDE_DIRS ${Boost_INCLUDE} ${libRadJav_INCLUDE_DIRS})
set (libRadJav_LINK_LIBRARIES ${Boost_LIBRARIES} ${libRadJav_LINK_LIBRARIES})

set (libRadJav_CPP_DEFS BOOST_SYSTEM_NO_DEPRECATED ${libRadJav_CPP_DEFS})
set (libRadJav_CPP_DEFS HAVE_WORKING_BOOST_SLEEP_FOR ${libRadJav_CPP_DEFS})
set (libRadJav_CPP_DEFS BOOST_SPIRIT_THREADSAFE ${libRadJav_CPP_DEFS})
set (libRadJav_CPP_DEFS USE_BOOST ${libRadJav_CPP_DEFS})

if (WIN32)
	set (libRadJav_CPP_DEFS WIN32_LEAN_AND_MEAN ${libRadJav_CPP_DEFS})
endif ()

link_directories (${Boost_LIBRARY_DIR_DEBUG} ${Boost_LIBRARY_DIR_RELEASE})

set (libRadJav_LINK_LIBRARY_DIRS ${Boost_LIBRARY_DIRS} ${libRadJav_LINK_LIBRARY_DIRS})

if (OGRE_FOUND)
	message (STATUS "Ogre 3D found...")
	set (libRadJav_INCLUDE_DIRS ${OGRE_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${OGRE_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_CPP_DEFS C3D_USE_OGRE ${libRadJav_CPP_DEFS})
endif ()

if (libRadJav_USE_OGRE_COLLADA)
	if (COLLADA_FOUND)
		message (STATUS "OgreColladaLoader found...")
		set (libRadJav_INCLUDE_DIRS ${COLLADA_INCLUDE} ${libRadJav_INCLUDE_DIRS})
		set (libRadJav_LINK_LIBRARIES ${COLLADA_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
		set (libRadJav_CPP_DEFS USE_OGRE_COLLADA ${libRadJav_CPP_DEFS})
	endif ()
endif ()

if (V8_FOUND)
	set (libRadJav_INCLUDE_DIRS ${V8_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_INCLUDE_DIRS ${ICU_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${V8_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_CPP_DEFS USE_V8 ${libRadJav_CPP_DEFS})
	message (STATUS "JavaScript engine: V8")
endif ()

if (JAVASCRIPTCORE_FOUND)
	set (libRadJav_CPP_DEFS USE_JAVASCRIPTCORE ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS_DEBUG USE_JAVASCRIPTCORE ${libRadJav_CPP_DEFS_DEBUG})
	set (libRadJav_LINK_LIBRARIES ${libRadJav_LINK_LIBRARIES} "-framework JavaScriptCore")
	message (STATUS "JavaScript engine: JavaScriptCore")
endif ()

#if (wxWidgets_FOUND)
#	include (FindwxWidgetsLib)
#endif ()

if (CURL_FOUND)
	set (libRadJav_INCLUDE_DIRS ${CURL_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${CURL_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_CPP_DEFS HTTP_USE_CURL ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS CURL_STATICLIB ${libRadJav_CPP_DEFS})
endif ()

if (nghttp2_FOUND)
	set (libRadJav_INCLUDE_DIRS ${nghttp2_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${nghttp2_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_CPP_DEFS HTTP_USE_NGHTTP2 ${libRadJav_CPP_DEFS})
endif ()

if (civetweb_FOUND)
	set (libRadJav_INCLUDE_DIRS ${civetweb_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${civetweb_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_CPP_DEFS HTTP_USE_CIVETWEB ${libRadJav_CPP_DEFS})
endif ()

if (libRadJav_INCLUDE_BLOCKCHAIN_V1)
	set (libRadJav_CPP_DEFS USE_BLOCKCHAIN_V1 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS HAVE_CONFIG_H ${libRadJav_CPP_DEFS})

	set (libRadJav_CPP_DEFS MINIUPNP_STATICLIB ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS STATICLIB ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS USE_UPNP ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS _CRT_SECURE_NO_WARNINGS ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS UNICODE ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS _SCL_SECURE_NO_WARNINGS ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS NOMINMAX ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS USE_IPV6=1 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS __STDC_FORMAT_MACROS ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS ENABLE_WALLET=1 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS __func__=__FUNCTION__ ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS USE_FIELD_10X26=1 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS USE_FIELD_INV_BUILTIN=1 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS USE_NUM_NONE=1 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS USE_SCALAR_8X32=1 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS USE_SCALAR_INV_BUILTIN=1 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS _LIB ${libRadJav_CPP_DEFS})
	
	set (INCLUDE_DATABASES true CACHE BOOL "Includes databases" FORCE)

	if (WIN32)
		set (libRadJav_CPP_DEFS _WIN32_WINNT=0x0501 ${libRadJav_CPP_DEFS})
	else ()
		add_compile_options (-enable-glibc-back-compat)
	endif ()

	include (FindModuleZLIB)
	include (FindBerkleyDB)
	include (FindModuleOpenSSL)
	include (FindlibEvent)
	include (FindMiniupnpc)
	include (Findsecp256k1)
	include (Findlibzmq)

	set (libRadJav_INCLUDE_DIRS "${libRadJav_SOURCE_DIR}/blockchainV1" ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_INCLUDE_DIRS "${libRadJav_SOURCE_DIR}/blockchainV1/src" ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_INCLUDE_DIRS "${libRadJav_SOURCE_DIR}/blockchainV1/src/config" ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_INCLUDE_DIRS "${libRadJav_SOURCE_DIR}/blockchainV1/src/univalue/include" ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_INCLUDE_DIRS "${libRadJav_SOURCE_DIR}/blockchainV1/src/secp256k1/include" ${libRadJav_INCLUDE_DIRS})

	set (libRadJav_INCLUDE_DIRS ${ZLIB_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	#set (libRadJav_LINK_LIBRARIES ${ZLIB_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_INCLUDE_DIRS ${BerkleyDB_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${BerkleyDB_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_INCLUDE_DIRS ${OPENSSL_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${OPENSSL_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_INCLUDE_DIRS ${event2_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${event2_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_INCLUDE_DIRS ${miniupnpc_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${miniupnpc_LIBRARIES} ${libRadJav_LINK_LIBRARIES})

	set (libRadJav_INCLUDE_DIRS ${secp256k1_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${secp256k1_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_INCLUDE_DIRS ${libzmq_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${libzmq_LIBRARIES} ${libRadJav_LINK_LIBRARIES})

	#if (MSVC)
	#	add_definitions(/FI stdafx.h)
	#endif ()

	if (WIN32)
		set(libRadJav_LINK_LIBRARIES debug Iphlpapi.lib optimized Iphlpapi.lib ${libRadJav_LINK_LIBRARIES})
	endif ()
endif ()

if (libRadJav_DEBUG_INSPECTOR)
	message (STATUS "V8 inspector support will be included in this build.")
	
	set (libRadJav_CPP_DEFS USE_INSPECTOR ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS_DEBUG USE_INSPECTOR ${libRadJav_CPP_DEFS_DEBUG})
	
else ()
	message (STATUS "V8 inspector support will NOT be included in this build.")
endif ()

if (FIND_MEMORY_LEAKS)
	set (libRadJav_CPP_DEFS_DEBUG LOG_MEMORY_LEAKS ${libRadJav_CPP_DEFS_DEBUG})
endif ()

if (INCLUDE_DATABASES)
	set (libRadJav_CPP_DEFS USE_DATABASE ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS_DEBUG USE_DATABASE ${libRadJav_CPP_DEFS_DEBUG})

	list(FIND databases ${libRadJav_DATABASE} index)
	if (index EQUAL -1)
		message (FATAL_ERROR "libRadJav_DATABASE must be selected and must be one of ${databases}")
	endif()

	message (STATUS "${libRadJav_DATABASE} selected as primary database to link with")
    	
	if (libRadJav_DATABASE STREQUAL "LevelDB")
		set (USE_SNAPPY true CACHE BOOL "Library links using Snappy?")

		include (Findleveldb)

		set (libRadJav_CPP_DEFS USE_LEVELDB ${libRadJav_CPP_DEFS})
		set (libRadJav_CPP_DEFS_DEBUG USE_LEVELDB ${libRadJav_CPP_DEFS_DEBUG})

		if (WIN32)
			set (libRadJav_CPP_DEFS LEVELDB_PLATFORM_WINDOWS ${libRadJav_CPP_DEFS})
			set (libRadJav_CPP_DEFS_DEBUG LEVELDB_PLATFORM_WINDOWS ${libRadJav_CPP_DEFS_DEBUG})
		else ()
			set (libRadJav_CPP_DEFS LEVELDB_PLATFORM_POSIX ${libRadJav_CPP_DEFS})
			set (libRadJav_CPP_DEFS_DEBUG LEVELDB_PLATFORM_POSIX ${libRadJav_CPP_DEFS_DEBUG})
		endif ()

		if (NOT WIN32)
			set (libRadJav_INCLUDE_DIRS ${leveldb_INCLUDE} ${libRadJav_INCLUDE_DIRS})
			set (libRadJav_LINK_LIBRARIES ${leveldb_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
		endif ()
	elseif (libRadJav_DATABASE STREQUAL "RocksDB" OR libRadJav_DATABASE STREQUAL "RocksDB and NuDB")
		set (USE_SNAPPY true CACHE BOOL "Library links using Snappy?")

		include (Findrocksdb)

		set (libRadJav_CPP_DEFS USE_ROCKSDB ${libRadJav_CPP_DEFS})
		set (libRadJav_CPP_DEFS_DEBUG USE_ROCKSDB ${libRadJav_CPP_DEFS_DEBUG})

		set (libRadJav_INCLUDE_DIRS ${rocksdb_INCLUDE} ${libRadJav_INCLUDE_DIRS})
		set (libRadJav_LINK_LIBRARIES ${rocksdb_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	elseif (libRadJav_DATABASE STREQUAL "NuDB" OR libRadJav_DATABASE STREQUAL "RocksDB and NuDB")
		include (Findnudb)

		set (libRadJav_CPP_DEFS USE_NUDB ${libRadJav_CPP_DEFS})
		set (libRadJav_CPP_DEFS_DEBUG USE_NUDB ${libRadJav_CPP_DEFS_DEBUG})

		set (libRadJav_INCLUDE_DIRS ${nudb_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	else ()
		message (FATAL_ERROR "Unknown libRadJav_DATABASE \"${libRadJav_DATABASE}\" specified")
	endif()

	if (USE_SNAPPY)
		set (libRadJav_CPP_DEFS USE_SNAPPY ${libRadJav_CPP_DEFS})
		set (libRadJav_CPP_DEFS_DEBUG USE_SNAPPY ${libRadJav_CPP_DEFS_DEBUG})
	endif ()
endif ()

if (INCLUDE_CRYPTOGRAPHY)
	set (OPENSSL_ROOT_DIR "" CACHE PATH "OPENSSL_ROOT_DIR")

	set (libRadJav_CPP_DEFS USE_CRYPTOGRAPHY ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS_DEBUG USE_CRYPTOGRAPHY ${libRadJav_CPP_DEFS_DEBUG})

	set (libRadJav_CPP_DEFS USE_OPENSSL ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS_DEBUG USE_OPENSSL ${libRadJav_CPP_DEFS_DEBUG})

	include (FindModuleOpenSSL)

	set (libRadJav_INCLUDE_DIRS ${OPENSSL_INCLUDE_DIR} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${OPENSSL_LIBRARIES} ${libRadJav_LINK_LIBRARIES})

	set (libKrispyCrypto_PATH "${libRadJav_PATH}/engines/KrispyCrypto" CACHE PATH "Crypto path")
	add_subdirectory (${libKrispyCrypto_PATH} ${libKrispyCrypto_PATH}/build)
	set (libRadJav_INCLUDE_DIRS ${libKrispyCrypto_INTERFACE_DIR} ${libRadJav_INCLUDE_DIRS})
endif ()

if (libRadJav_ALLOW_NETWORKING)
	set (libRadJav_CPP_DEFS NET_ON ${libRadJav_CPP_DEFS})
endif ()

set (libRadJav_CPP_DEFS LIBRADJAV ${libRadJav_CPP_DEFS})

if (wxWidgets_FOUND)
	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		set (libRadJav_CPP_DEFS_DEBUG WXDEBUG=1 ${libRadJav_CPP_DEFS_DEBUG})
		set (libRadJav_CPP_DEFS_DEBUG __WXDEBUG__ ${libRadJav_CPP_DEFS_DEBUG})
	endif ()

	if (WEBVIEW_TYPE STREQUAL "wxWidgetsDefault")
		set (USE_WXWIDGETS_WEBVIEW true)
		set (libRadJav_CPP_DEFS_DEBUG WXWIDGETS_HAS_WEBVIEW ${libRadJav_CPP_DEFS_DEBUG})
		set (libRadJav_CPP_DEFS WXWIDGETS_HAS_WEBVIEW ${libRadJav_CPP_DEFS})
	endif ()

	set (libRadJav_CPP_DEFS GUI_USE_WXWIDGETS ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS ${wxWidgets_DEFINITIONS} ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS_DEBUG ${wxWidgets_DEFINITIONS_DEBUG} ${libRadJav_CPP_DEFS_DEBUG})
	set (libRadJav_CXX_FLAGS ${wxWidgets_CXX_FLAGS} ${libRadJav_CXX_FLAGS})
	set (libRadJav_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${wxWidgets_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
	set (libRadJav_LINK_LIBRARY_DIRS ${wxWidgets_LIBRARY_DIRS} ${libRadJav_LINK_LIBRARY_DIRS})
endif ()

if (XML_SUPPORT)
	set (libRadJav_CPP_DEFS USE_TINYXML2 ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS_DEBUG USE_TINYXML2 ${libRadJav_CPP_DEFS_DEBUG})
	set (libRadJav_CPP_DEFS HAS_XML_SUPPORT ${libRadJav_CPP_DEFS})
	set (libRadJav_CPP_DEFS_DEBUG HAS_XML_SUPPORT ${libRadJav_CPP_DEFS_DEBUG})

	set (libRadJav_INCLUDE_DIRS ${TinyXML2_INCLUDE} ${libRadJav_INCLUDE_DIRS})
	set (libRadJav_LINK_LIBRARIES ${TinyXML2_LIBRARIES} ${libRadJav_LINK_LIBRARIES})
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set (libRadJav_CPP_DEFS_DEBUG _DEBUG ${libRadJav_CPP_DEFS_DEBUG})
	set (libRadJav_CPP_DEFS_DEBUG ${libRadJav_CPP_DEFS} ${libRadJav_CPP_DEFS_DEBUG})
endif ()

if (MSVC)
	add_definitions (/wd4996)
	#set (CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Od /MP")
	#set (CMAKE_CXX_FLAGS_RELEASE "/MT /MP")
	#set (CMAKE_CXX_FLAGS_MINSIZEREL "/MT /MP")
	#set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /MP")
	set (CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Od /MP")
	set (CMAKE_CXX_FLAGS_RELEASE "/MD /MP")
	set (CMAKE_CXX_FLAGS_MINSIZEREL "/MD /MP")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /MP")
endif ()

if (UNIX)
	add_compile_options (-std=c++11)
	add_compile_options (-fpermissive)
endif ()

# Main sources
include (includes)
include (sources)

# Header file RadJavJavascriptCode.h will be generated using CMake, see below
set (SOURCES ${libRadJav_SOURCE_DIR}/include/RadJav/RadJavJavascriptCode.h ${SOURCES})

# Create static/shared library for RadJav
if (libRadJav_STATIC)
	add_library (libRadJav STATIC ${SOURCES})
else ()
	add_library (libRadJav SHARED ${SOURCES})
endif ()

include_directories (${libRadJav_INCLUDE_DIRS})
target_link_libraries (libRadJav ${libRadJav_LINK_LIBRARIES})

set_target_properties (libRadJav PROPERTIES LINKER_LANGUAGE CXX)
set_property (TARGET libRadJav APPEND PROPERTY COMPILE_DEFINITIONS ${libRadJav_CPP_DEFS})
set_property (TARGET libRadJav APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:${libRadJav_CPP_DEFS} ${libRadJav_CPP_DEFS_DEBUG}>)

if (WIN32)
	set_target_properties (libRadJav PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
	set_target_properties (libRadJav PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
	set_target_properties (libRadJav PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties (libRadJav PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif ()

set_property (TARGET libRadJav PROPERTY PROJECT_LABEL "libRadJav")

if (libRadJav_EMBEDFILE)
	if (NOT libRadJav_EMBEDFILE_CMAKE)
	#<TODO: Comment these to use CMake for embedding Javascript files into RadJavJavascriptCode.h>
	add_custom_command (OUTPUT ${libRadJav_SOURCE_DIR}/include/RadJav/RadJavJavascriptCode.h
		COMMAND RadJavCompileResources
		DEPENDS ${libRadJav_SOURCE_DIR}/javascript/RadJav.js)

	add_executable (RadJavCompileResources ${libRadJav_SOURCE_DIR}/src/RadJav/RadJavCompileResources.cpp)
	set_property (TARGET RadJavCompileResources APPEND PROPERTY COMPILE_DEFINITIONS ${libRadJav_CPP_DEFS})
	set_property (TARGET RadJavCompileResources APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:${libRadJav_CPP_DEFS} ${libRadJav_CPP_DEFS_DEBUG}>)

	add_dependencies (libRadJav RadJavCompileResources)
	#</TODO>
	endif()

	set (jsFiles Math.js
			String.js
			RadJav.js
			RadJav.Color.js
			RadJav.Quaternion.js
			RadJav.Vector2.js
			RadJav.Vector3.js
			RadJav.Vector4.js
			RadJav.Circle.js
			RadJav.Rectangle.js
			RadJav.Font.js
			RadJav.Thread.js
			RadJav.Animation.js
			RadJav.IO.js
			RadJav.Interact.js
			RadJav.Testing.js
	)
	
	if (INCLUDE_DATABASES)
		set (jsFiles ${jsFiles}
				RadJav.DB.KeyValueStorage.js
		)
	endif ()
	
	if (INCLUDE_CRYPTOGRAPHY)
		set (jsFiles ${jsFiles}
				RadJav.Crypto.Cipher.js
				RadJav.Crypto.Decipher.js
				RadJav.Crypto.CipherMultipart.js
				RadJav.Crypto.DecipherMultipart.js
				RadJav.Crypto.KeyGenerator.js
				RadJav.Crypto.PrivateKey.js
				RadJav.Crypto.PublicKey.js
				RadJav.Crypto.Hash.js
				RadJav.Crypto.HashMultipart.js
		)
	endif ()
	
	

	if (IS_MOBILE)
		set (jsFiles ${jsFiles}
			RadJav.GUI.GObject.js # This must be compiled before any other GUI object.
			RadJav.MUI.ViewController.js
			RadJav.MUI.NavigationViewController.js
			RadJav.MUI.View.js
			RadJav.MUI.ScrollView.js
			RadJav.MUI.Button.js
			RadJav.MUI.Label.js
			RadJav.MUI.Image.js
			RadJav.MUI.Checkbox.js
			RadJav.MUI.Textbox.js
			RadJav.MUI.TableViewController.js
			RadJav.MUI.TableView.js
			RadJav.MUI.Textarea.js
		)

	endif ()

	if (USE_WXWIDGETS)
		set (jsFiles ${jsFiles}
			RadJav.GUI.GObject.js # This must be compiled before any other GUI object.
			RadJav.GUI.Button.js
			RadJav.GUI.Checkbox.js
			RadJav.GUI.Combobox.js
			RadJav.GUI.Container.js
			RadJav.GUI.Image.js
			RadJav.GUI.Label.js
			RadJav.GUI.List.js
			RadJav.GUI.Radio.js
			RadJav.GUI.Textarea.js
			RadJav.GUI.Textbox.js
			RadJav.GUI.Window.js
			RadJav.GUI.MenuBar.js
			RadJav.GUI.MenuItem.js
			RadJav.GUI.KeyEvent.js
			RadJav.GUI.MouseEvent.js
		)
		
		if (USE_WXWIDGETS_WEBVIEW)
			set (jsFiles ${jsFiles}
				RadJav.GUI.WebView.js
			)
		endif ()
	endif ()

	if (libRadJav_ALLOW_NETWORKING)
		set (jsFiles ${jsFiles}
			RadJav.Net.WebSocketServer.js
			RadJav.Net.WebSocketClient.js
			RadJav.Net.WebServer.js
		)
	endif ()

	
	if (USE_OGRE)
		set (jsFiles ${jsFiles}
				RadJav.GUI.Canvas3D.js
				RadJav.C3D.Transform.js # This must be compiled before any other C3D object.
				RadJav.C3D.Object3D.js
				RadJav.C3D.Plane.js
				RadJav.C3D.Cube.js
				RadJav.C3D.Sphere.js
				RadJav.C3D.Camera.js
				RadJav.C3D.Light.js
				RadJav.C3D.Material.js
				RadJav.C3D.Model.js
		)
	endif ()
	
	if (libRadJav_EMBEDFILE_CMAKE)
		#TODO: Un-comment these to use CMake for embedding Javascript files into RadJavJavascriptCode.h
		include (EmbedJavascript)
	
		embedJavascript( "${jsFiles}" "${CMAKE_CURRENT_SOURCE_DIR}/javascript" "RadJavJavascriptCode.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/RadJav")
	endif ()
endif()

set (libRadJav_CPP_DEFS2 ${libRadJav_CPP_DEFS} PARENT_SCOPE)
set (libRadJav_CPP_DEFS_DEBUG2 ${libRadJav_CPP_DEFS_DEBUG} PARENT_SCOPE)
set (libRadJav_INCLUDE_DIRS2 ${libRadJav_INCLUDE_DIRS} PARENT_SCOPE)
set (libRadJav_LINK_LIBRARIES2 ${libRadJav_LINK_LIBRARIES} PARENT_SCOPE)
set (libRadJav_LINK_LIBRARY_DIRS2 ${libRadJav_LINK_LIBRARY_DIRS} PARENT_SCOPE)
set (libRadJav_CXX_FLAGS2 ${libRadJav_CXX_FLAGS} PARENT_SCOPE)
set (libRadJav_USE_V8 ${USE_V8} PARENT_SCOPE)
set (CMAKE_CXX_FLAGS2 ${CMAKE_CXX_FLAGS} PARENT_SCOPE)

