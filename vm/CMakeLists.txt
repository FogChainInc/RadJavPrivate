cmake_minimum_required (VERSION 3.0)

if (WIN32)
	set (CMAKE_USE_RELATIVE_PATHS TRUE)
endif ()

project (RadJavVM)

message (STATUS "Configuring RadJav VM...")
message (STATUS "Version 0.1")

set (PROJECT_VERSION_MAJOR "0")
set (PROJECT_VERSION_MINOR "1")
set (PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

set (CMAKE_MODULE_PATH "${RadJavVM_SOURCE_DIR}/../library/CMake"
			"${RadJavVM_SOURCE_DIR}/../library/CMake/lib"
			"${RadJavVM_SOURCE_DIR}/CMake"
			"${RadJavVM_SOURCE_DIR}/include"
			"${RadJavVM_SOURCE_DIR}/src"
			"${RadJavVM_SOURCE_DIR}/res")

include (Utils)

set (CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR} CACHE STRING "CMAKE_SYSTEM_PROCESSOR")
set (CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "CMAKE_CXX_FLAGS_DEBUG")
set (CMAKE_CXX_FLAGS_RELEASE "" CACHE STRING "CMAKE_CXX_FLAGS_RELEASE")
set (CMAKE_CXX_FLAGS_MINSIZEREL "" CACHE STRING "CMAKE_CXX_FLAGS_MINSIZEREL")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE STRING "CMAKE_CXX_FLAGS_RELWITHDEBINFO")
set (RadJavVM_CPP_DEFS "" CACHE STRING "C++ Definitions")
set (RadJavVM_CPP_DEFS_DEBUG "" CACHE STRING "C++ Debug Definitions")
set (libRadJav_PATH $ENV{RADJAV_PATH} CACHE PATH "RadJav path")
set (libRadJav_CMAKE_BUILD_PATH "" CACHE PATH "libRadJav CMake Build Path")
set (libRadJav_DEPENDENCIES $ENV{RADJAV_DEPENDENCIES} CACHE PATH "RadJav Dependencies path")
set (RadJavVM_LANGUAGE "en_us" CACHE STRING "Select language to compile with")
set (libRadJav_STATIC false CACHE BOOL "Use static build of libRadJav")
set (RadJavVM_CREATE_INSTALL_PACKAGES false CACHE BOOL "Create install packages")
set (RadJavVM_EMBED_EXAMPLES true CACHE BOOL "Embed examples into application bundle")
set (RadJavVM_EMBED_HTML5 false CACHE BOOL "Embed HTML5 into application bundle")

if (libRadJav_JS_LIBRARY STREQUAL "V8")
	set (RadJavVM_DEPENDENCY_OBJECTS_LIST "./natives_blob.bin;./snapshot_blob.bin" CACHE STRING "Semicolon separated list of paths to dependencies \
												which are indirectly will be used by application, i.e. loading \
												during runtime")
else ()
	set (RadJavVM_DEPENDENCY_OBJECTS_LIST "" CACHE STRING "Semicolon separated list of paths to dependencies \
												which are indirectly will be used by application, i.e. loading \
												during runtime")
endif ()

if (APPLE)
	set (MAC_CODESIGN_IDENTITY CACHE STRING "Sign identity which can be determined by issuing command:\
							security find-identity -v -p codesigning\
							Example: Mac Developer")
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENSSL_CRYPTO_LIBRARY "${RadJavVM_SOURCE_DIR}/../../sdk/openssl/build/install/lib/libcrypto.so" CACHE FILEPATH "libcrypto" FORCE) 
set(OPENSSL_SSL_LIBRARY "${RadJavVM_SOURCE_DIR}/../../sdk/openssl/build/install/lib/libssl.so" CACHE FILEPATH "libssl" FORCE) 
set(OPENSSL_INCLUDE_DIR "${RadJavVM_SOURCE_DIR}/../../sdk/openssl/build/install/include" CACHE PATH "OpenSSL include" FORCE)
endif()

message (STATUS "Building for processor type: ${CMAKE_SYSTEM_PROCESSOR}")

set (RadJavVM_LANGUAGE_UPPER)
string (TOUPPER ${RadJavVM_LANGUAGE} RadJavVM_LANGUAGE_UPPER)
message (STATUS "Using language ${RadJavVM_LANGUAGE_UPPER}")

set (CMAKE_DEBUG_POSTFIX "_d")
#set (RadJavVM_SOURCE_LANGUAGE_FILE)

#set (SOURCES_files_Languages
#	"${RadJavVM_SOURCE_DIR}/include/RadJav/languages/${RadJavVM_LANGUAGE}.h")
#source_group ("Languages" FILES ${SOURCES_files_Languages})
set (RadJavVM_SOURCES "${RadJavVM_SOURCE_DIR}/src/RadJavVM" ${RadJavVM_SOURCES})

#set (RadJavVM_SOURCES ${SRC} ${RadJavVM_SOURCES})

include (includes)
include (sources)
include (res)

include (FindRadJav)

set (RadJavVM_INCLUDE_DIRS)
set (RadJavVM_LINK_LIBRARIES)
set (RadJavVM_LINK_LIBRARY_DIRS)
set (RadJavVM_CXX_FLAGS)

set (RadJavVM_INCLUDE_DIRS "${RadJavVM_SOURCE_DIR}/include/RadJavVM" ${RadJavVM_INCLUDE_DIRS})

if (libRadJav_FOUND)
	message (STATUS "libRadJav Library Found...")
	set (RadJavVM_INCLUDE_DIRS ${libRadJav_INCLUDE} ${RadJavVM_INCLUDE_DIRS})
	set (RadJavVM_LINK_LIBRARIES ${libRadJav_LIBRARIES} ${RadJavVM_LINK_LIBRARIES})

	if (libRadJav_CMAKE_BUILD_PATH)
		add_subdirectory (${libRadJav_PATH} ${libRadJav_CMAKE_BUILD_PATH})
	else ()
		add_subdirectory (${libRadJav_PATH} ${libRadJav_PATH}/build)
	endif ()

	set (RadJavVM_CPP_DEFS_DEBUG ${libRadJav_CPP_DEFS_DEBUG2} ${RadJavVM_CPP_DEFS_DEBUG})
	set (RadJavVM_CPP_DEFS ${libRadJav_CPP_DEFS2} ${RadJavVM_CPP_DEFS})
	set (RadJavVM_INCLUDE_DIRS ${libRadJav_INCLUDE_DIRS2} ${RadJavVM_INCLUDE_DIRS})
	set (RadJavVM_LINK_LIBRARIES ${libRadJav_LINK_LIBRARIES2} ${RadJavVM_LINK_LIBRARIES})
	set (RadJavVM_LINK_LIBRARY_DIRS ${libRadJav_LINK_LIBRARY_DIRS2} ${RadJavVM_LINK_LIBRARY_DIRS})
	set (RadJavVM_JS_LIBRARY ${libRadJav_JS_LIBRARY2} ${RadJavVM_JS_LIBRARY})
	set (RadJavVM_CXX_FLAGS ${libRadJav_CXX_FLAGS2} ${RadJavVM_CXX_FLAGS})
	set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS2} ${CMAKE_CXX_FLAGS})

	if (WIN32)
		list (REMOVE_ITEM RadJavVM_CPP_DEFS WIN32_LEAN_AND_MEAN)
		list (REMOVE_ITEM RadJavVM_CPP_DEFS_DEBUG WIN32_LEAN_AND_MEAN)
	endif ()
endif ()

if (MSVC)
	add_definitions (/wd4996)
	#set (CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Od")
	#set (CMAKE_CXX_FLAGS_RELEASE "/MT")
	#set (CMAKE_CXX_FLAGS_MINSIZEREL "/MT")
	#set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT")
	set (CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Od /MP")
	set (CMAKE_CXX_FLAGS_RELEASE "/MD /MP")
	set (CMAKE_CXX_FLAGS_MINSIZEREL "/MD /MP")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /MP")
endif ()

if (libRadJav_STATIC)
	set (RadJavVM_CPP_DEFS_DEBUG RADJAV_LIB_STATIC ${RadJavVM_CPP_DEFS_DEBUG})
	set (RadJavVM_CPP_DEFS RADJAV_LIB_STATIC ${RadJavVM_CPP_DEFS})
else ()
	set (RadJavVM_CPP_DEFS_DEBUG RADJAV_LIB_DLL ${RadJavVM_CPP_DEFS_DEBUG})
	set (RadJavVM_CPP_DEFS RADJAV_LIB_DLL ${RadJavVM_CPP_DEFS})
endif ()

# Create executable for RadJav.
set (ISWIN32)
set (ISOSX)

if (WIN32)
	set (ISWIN32 "WIN32")
	set (RadJavVM_LINK_LIBRARIES ws2_32 wldap32 ${RadJavVM_LINK_LIBRARIES})
endif ()

set (CMAKE_C_FLAGS ${RadJavVM_CXX_FLAGS} ${CMAKE_C_FLAGS})


if (INCLUDE_CRYPTOGRAPHY)
	set (RadJavVM_LINK_LIBRARIES ${libKrispyCrypto_LIBRARIES} ${RadJavVM_LINK_LIBRARIES})
	#target_link_libraries ( ${CMAKE_PROJECT_NAME} ${libKrispyCrypto_NAME})
endif ()

link_directories (${RadJavVM_LINK_LIBRARY_DIRS})

add_executable (${CMAKE_PROJECT_NAME} ${ISWIN32} ${ISOSX} ${RadJavVM_SOURCES})

if (UNIX)
	if (NOT IOS)
		target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -std=c++11)
	endif ()
	target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -fpermissive)
endif ()

target_include_directories (${CMAKE_PROJECT_NAME} PUBLIC ${RadJavVM_INCLUDE_DIRS})

target_link_libraries (${CMAKE_PROJECT_NAME} libRadJav)
target_link_libraries (${CMAKE_PROJECT_NAME} ${RadJavVM_LINK_LIBRARIES})

set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES COMMAND_ARGUMENTS "${libRadJav_PATH}/../examples/window/window.js")

set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_property (TARGET ${CMAKE_PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${RadJavVM_CPP_DEFS})
set_property (TARGET ${CMAKE_PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:${RadJavVM_CPP_DEFS} ${RadJavVM_CPP_DEFS_DEBUG}>)

set (CMAKE_PROJECT_NAME_LAUNCHER ${CMAKE_PROJECT_NAME})

#Set destinations of a bundle
message(STATUS "Binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Project name: ${CMAKE_PROJECT_NAME}")

#Copying dependencies
foreach (dependency ${RadJavVM_DEPENDENCY_OBJECTS_LIST})
	add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
				COMMAND ${CMAKE_COMMAND} -E copy_if_different ${dependency} ${CMAKE_CURRENT_BINARY_DIR}
				COMMENT "Adding extra dependency into application bundle: ${dependency}"
				VERBATIM )
endforeach (dependency)

if (RadJavVM_EMBED_HTML5)
	message(STATUS "HTML5 platform will be built...")

	add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
				COMMAND npm install
				WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../html5/" 
				VERBATIM )
	add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
				COMMAND npm run build-radjav doNotCopyFilesToLibrary 
				WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../html5/" 
				COMMENT "Building HTML5..." 
				VERBATIM )
endif ()

if (WIN32)
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
elseif (APPLE)
	if (IOS)
		set (APP_BUNDLE_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.app")
		set (APP_BUNDLE_ICON_FILE "RadJav.icns")
	
		#Creating Launcher
		#TODO: add conditional for frameworks here
		target_link_libraries (${CMAKE_PROJECT_NAME} "-framework UIKit -framework JavaScriptCore")
	
		#These needs to be defined outside of set_target_properties otherwise they will not be included
		set (MACOSX_BUNDLE_DISPLAY_NAME ${CMAKE_PROJECT_NAME})
		set (MACOSX_BUNDLE_SIGNATURE "rjvm")
		
		#Configuring Info.plist for Mac Bundle
		set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE
									MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME}
									MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
									MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION}
									MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
									MACOSX_BUNDLE_ICON_FILE ${APP_BUNDLE_ICON_FILE}
									MACOSX_BUNDLE_GUI_IDENTIFIER "com.highersoftware.${CMAKE_PROJECT_NAME}"
									MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/CMake/Info.plist.ios.in")
		
		#Copying icons
		add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/installers/mac/${APP_BUNDLE_ICON_FILE} $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}>/${APP_BUNDLE_ICON_FILE}
					COMMENT "Copying icons into application bundle")
		
		#Embed examples
		if (RadJavVM_EMBED_EXAMPLES)
			message (STATUS "Examples will be embedded into application bundle")
			
			#Copying examples
			add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
						COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/../examples" $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}>/examples
						COMMENT "Copying examples into application bundle")
		endif ()
		
	else ()
		#Set destinations of a bundle
		set (CMAKE_PROJECT_NAME_LAUNCHER ${CMAKE_PROJECT_NAME}Launcher)
		
		set (APP_BUNDLE_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME_LAUNCHER}.app")
		set (APP_CONTENTS_DIR "${APP_BUNDLE_LOCATION}/Contents")
		set (APP_EXECUTABLE_DIR "${APP_CONTENTS_DIR}/MacOS")
		set (APP_RESOURCES_DIR "${APP_CONTENTS_DIR}/Resources")
		set (APP_BUNDLE_ICON_FILE "RadJav.icns")
	
		#Creating Launcher
		add_executable (${CMAKE_PROJECT_NAME_LAUNCHER} ${RadJavVMLauncher_SOURCES})
		target_link_libraries (${CMAKE_PROJECT_NAME_LAUNCHER} "-framework Cocoa")
		add_dependencies (${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME_LAUNCHER})
	
		#These needs to be defined outside of set_target_properties otherwise they will not be included
		set (MACOSX_BUNDLE_DISPLAY_NAME ${CMAKE_PROJECT_NAME_LAUNCHER})
		set (MACOSX_BUNDLE_SIGNATURE "rjvm")
		set (MACOSX_BUNDLE_MIN_SYSTEM_VERSION "10.13")
		
		#Configuring Info.plist for Mac Bundle
		set_target_properties (${CMAKE_PROJECT_NAME_LAUNCHER} PROPERTIES MACOSX_BUNDLE TRUE
										MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME_LAUNCHER}
										MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
										MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION}
										MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
										MACOSX_BUNDLE_ICON_FILE ${APP_BUNDLE_ICON_FILE}
										MACOSX_BUNDLE_COPYRIGHT "© 2018, Higher Edge Software"
										MACOSX_BUNDLE_GUI_IDENTIFIER "com.highersoftware.${CMAKE_PROJECT_NAME}"
										MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/CMake/Info.plist.in")
		
		#Copying icons
		add_custom_command (TARGET ${CMAKE_PROJECT_NAME_LAUNCHER} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/installers/mac/${APP_BUNDLE_ICON_FILE} ${APP_RESOURCES_DIR}/${APP_BUNDLE_ICON_FILE}
					COMMENT "Copying icons into application bundle")
		
		#Place VM into correct bundle folder
		set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${APP_EXECUTABLE_DIR}
									RUNTIME_OUTPUT_DIRECTORY_RELEASE ${APP_EXECUTABLE_DIR}
									RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${APP_EXECUTABLE_DIR}
									RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${APP_EXECUTABLE_DIR})
		
		#Embed examples
		if (RadJavVM_EMBED_EXAMPLES)
			message (STATUS "Examples will be embedded into application bundle")
			
			#Copying examples
			add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
						COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/../examples" "${APP_RESOURCES_DIR}/examples"
						COMMENT "Copying examples into application bundle"
						VERBATIM )
		endif ()
	endif ()
endif ()

set_property (TARGET ${CMAKE_PROJECT_NAME} PROPERTY PROJECT_LABEL ${CMAKE_PROJECT_NAME})
add_dependencies (${CMAKE_PROJECT_NAME} libRadJav)

if (${CMAKE_VERSION} VERSION_GREATER "3.6.0")
	if (MSVC)
		set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
	endif ()
endif ()

if (RadJavVM_CREATE_INSTALL_PACKAGES)
	#Install
	include (InstallRadJavVM)

	#Packaging
	include (PackageRadJavVM)
endif ()
