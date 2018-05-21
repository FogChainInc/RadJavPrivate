cmake_minimum_required (VERSION 3.0)

if (WIN32)
	set (CMAKE_USE_RELATIVE_PATHS TRUE)
endif ()

#Set CMAKE_MACOSX_BUNDLE to YES if we plan to make RadJavVM as a BUNDLE for Apple
if (APPLE)
	set (CMAKE_MACOSX_BUNDLE NO)
endif ()

project (RadJavVM)

message (STATUS "Configuring RadJav VM...")
message (STATUS "Version 0.1")

set (CMAKE_MODULE_PATH "${RadJavVM_SOURCE_DIR}/../library/CMake"
			"${RadJavVM_SOURCE_DIR}/../library/CMake/lib"
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
	set (RadJavVM_CXX_FLAGS ${libRadJav_CXX_FLAGS2} ${RadJavVM_CXX_FLAGS})

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

if (UNIX)
	add_compile_options (-std=c++11)
	add_compile_options (-fpermissive)
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

include_directories (${RadJavVM_INCLUDE_DIRS})
link_directories (${RadJavVM_LINK_LIBRARY_DIRS})

add_executable (RadJavVM ${ISWIN32} ${ISOSX} ${RadJavVM_SOURCES})

target_link_libraries (RadJavVM libRadJav)
target_link_libraries (RadJavVM ${RadJavVM_LINK_LIBRARIES})
set_target_properties (RadJavVM PROPERTIES COMMAND_ARGUMENTS "${libRadJav_PATH}/../examples/window/window.js")

set_target_properties (RadJavVM PROPERTIES LINKER_LANGUAGE CXX)
set_property (TARGET RadJavVM APPEND PROPERTY COMPILE_DEFINITIONS ${RadJavVM_CPP_DEFS})
set_property (TARGET RadJavVM APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:${RadJavVM_CPP_DEFS} ${RadJavVM_CPP_DEFS_DEBUG}>)

if (WIN32)
	set_target_properties (RadJavVM PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
	set_target_properties (RadJavVM PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
	set_target_properties (RadJavVM PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties (RadJavVM PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif ()

set_property (TARGET RadJavVM PROPERTY PROJECT_LABEL "RadJavVM")
add_dependencies (RadJavVM libRadJav)

if (${CMAKE_VERSION} VERSION_GREATER "3.6.0")
	if (MSVC)
		set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RadJavVM)
	endif ()
endif ()

#if (libRadJav_EMBEDFILE)
#	add_custom_command (OUTPUT ${libRadJav_PATH}/include/RadJav/RadJavJavascriptCode.h
#		COMMAND RadJavCompileResources
#		DEPENDS ${libRadJav_PATH}/javascript/RadJav.js)

#	add_executable (RadJavCompileResources ${libRadJav_PATH}/src/RadJav/RadJavCompileResources.cpp)
#	set_property (TARGET RadJavCompileResources APPEND PROPERTY COMPILE_DEFINITIONS ${RadJavVM_CPP_DEFS})
#	set_property (TARGET RadJavCompileResources APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:${RadJavVM_CPP_DEFS} ${RadJavVM_CPP_DEFS_DEBUG}>)
#endif ()
