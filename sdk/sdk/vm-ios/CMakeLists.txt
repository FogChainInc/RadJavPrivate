cmake_minimum_required (VERSION 3.0)

# Create cache variables of passed to CMake arguments we require
set (APP_NAME ${RJ_PROJECT_NAME} CACHE STRING "Name of executable")
set (APP_PATH "${RJ_APP_PATH}" CACHE STRING "Path to JavaScript application")
set (DEPENDENCIES_PATH "${RJ_DEPENDENCIES_PATH}" CACHE STRING "Path to precompiled dependencies")
set (BUNDLE_IDENTIFIER "${RJ_BUNDLE_IDENTIFIER}" CACHE STRING "Bundle identifier, required for packaging")
set (BUNDLE_SIGNATURE ${RJ_BUNDLE_SIGNATURE} CACHE STRING "Bundle signature(4 bytes), required for packaging")
set (XCODE_DEVTEAM_ID "${RJ_DEVELOPMENT_TEAM_ID}" CACHE STRING "The developer team id")
set (XCODE_CODESIGN_IDENTITY "${RJ_CODESIGN_IDENTITY}" CACHE STRING "Sign identity which can be determined by issuing command:\
						security find-identity -v -p codesigning\
						Get the hash from Example: Mac Developer")
set (XCODE_PROVISIONING_PROFILE_NAME "${RJ_PROVISIONING_PROFILE_NAME}" CACHE STRING "The provisioning profile name")

mark_as_advanced(FORCE APP_NAME APP_PATH DEPENDENCIES_PATH)

# Specify version parts
set (VERSION_MAJOR "0" CACHE STRING "Major version")
set (VERSION_MINOR "1" CACHE STRING "Minor version")
set (VERSION_PATCH "0" CACHE STRING "Patch version")

# Create project
project (${APP_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

message (STATUS "Configuring ${CMAKE_PROJECT_NAME}...")
message (STATUS "${CMAKE_PROJECT_NAME} version ${PROJECT_VERSION}")

# TODO: should it be ovewritten by user needs?
set (VENDOR "FogChain, Corp")

# Specify where CMake modules reside
set (CMAKE_MODULE_PATH "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/CMake"
			"${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include"
			"${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/src"
 			"${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/res")

# Instruct CMake where to find libs/headers of dependencies
set (CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} ${DEPENDENCIES_PATH})

# Specify architecture we build for
set (CMAKE_LIBRARY_ARCHITECTURE ${CMAKE_OSX_ARCHITECTURES})

# Specify postfix for debug libs
set (CMAKE_DEBUG_POSTFIX "_d")

# Generic variables
set (CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR} CACHE STRING "CMAKE_SYSTEM_PROCESSOR")
set (CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "CMAKE_CXX_FLAGS_DEBUG")
set (CMAKE_CXX_FLAGS_RELEASE "" CACHE STRING "CMAKE_CXX_FLAGS_RELEASE")
set (CMAKE_CXX_FLAGS_MINSIZEREL "" CACHE STRING "CMAKE_CXX_FLAGS_MINSIZEREL")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE STRING "CMAKE_CXX_FLAGS_RELWITHDEBINFO")
set (${CMAKE_PROJECT_NAME}_CPP_DEFS "" CACHE STRING "C++ Definitions")
set (${CMAKE_PROJECT_NAME}_CPP_DEFS_DEBUG "" CACHE STRING "C++ Debug Definitions")

include (includes)
include (sources)
include (res)

# Search for RadJav library
include (FindRadJav)

include_directories ("./include")
include_directories (${radjav_INCLUDE_DIR})

# Search for Boost libraries
find_package (Boost 1.67.0 REQUIRED COMPONENTS thread system filesystem chrono)
include_directories (Boost::boost)

#set (${CMAKE_PROJECT_NAME}_CPP_DEFS_DEBUG RADJAV_LIB_STATIC ${${CMAKE_PROJECT_NAME}_CPP_DEFS_DEBUG})
#set (${CMAKE_PROJECT_NAME}_CPP_DEFS RADJAV_LIB_STATIC ${${CMAKE_PROJECT_NAME}_CPP_DEFS})

# Define executable
add_executable (${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_SOURCES})
set_property (TARGET ${CMAKE_PROJECT_NAME} PROPERTY PROJECT_LABEL ${CMAKE_PROJECT_NAME})
set_property (TARGET ${CMAKE_PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${CMAKE_PROJECT_NAME}_CPP_DEFS})
set_property (TARGET ${CMAKE_PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:${${CMAKE_PROJECT_NAME}_CPP_DEFS} ${${CMAKE_PROJECT_NAME}_CPP_DEFS_DEBUG}>)

#libRadJav flags
target_compile_definitions (${CMAKE_PROJECT_NAME} PUBLIC LIBRADJAV
							USE_JAVASCRIPTCORE
							USE_BOOST
							BOOST_SPIRIT_THREADSAFE
							HAVE_WORKING_BOOST_SLEEP_FOR
							BOOST_SYSTEM_NO_DEPRECATED
							LINUX
							USE_IOS
							RADJAV_LIB_STATIC
							LANG_EN_US
							THREADS_USE_STD_THREAD)

target_compile_options (${CMAKE_PROJECT_NAME} PUBLIC -fpermissive)

# Dependencies with which we link
target_link_libraries (${CMAKE_PROJECT_NAME} debug ${radjav_LIBRARY_DEBUG})
target_link_libraries (${CMAKE_PROJECT_NAME} optimized ${radjav_LIBRARY_RELEASE})
target_link_libraries (${CMAKE_PROJECT_NAME} Boost::filesystem Boost::system Boost::thread Boost::chrono)
target_link_libraries (${CMAKE_PROJECT_NAME} -lc++)
target_link_libraries (${CMAKE_PROJECT_NAME} "-framework UIKit -framework JavaScriptCore -framework WebKit")


#Configuring Xcode
set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES
						XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${BUNDLE_IDENTIFIER}
						XCODE_ATTRIBUTE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}
						XCODE_ATTRIBUTE_SKIP_INSTALL "NO"
						XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
						XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${XCODE_DEVTEAM_ID}"
						XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${XCODE_CODESIGN_IDENTITY}"
						XCODE_ATTRIBUTE_PROVISIONING_PROFILE_SPECIFIER "${XCODE_PROVISIONING_PROFILE_NAME}")

set (APP_BUNDLE_ICON_FILE "App.icns")

#These needs to be defined outside of set_target_properties otherwise they will not be included
set (MACOSX_BUNDLE_DISPLAY_NAME ${CMAKE_PROJECT_NAME})
if (NOT BUNDLE_SIGNATURE)
	set (MACOSX_BUNDLE_SIGNATURE "????")
else ()
	set (MACOSX_BUNDLE_SIGNATURE ${BUNDLE_SIGNATURE})
endif ()

#Configuring Info.plist for Mac Bundle
set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE
							MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME}
							MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
							MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION}
							MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
							MACOSX_BUNDLE_ICON_FILE ${APP_BUNDLE_ICON_FILE}
							MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER}
							MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/CMake/Info.plist.in")

#Copying icons
add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/res/${APP_BUNDLE_ICON_FILE} $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}>/${APP_BUNDLE_ICON_FILE}
			COMMENT "Copying icons into application bundle")

#Embed Javascript application
add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
			COMMAND ${CMAKE_COMMAND} -E copy_directory "${APP_PATH}" $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}>/app
			COMMENT "Copying Javascript application into application bundle")

