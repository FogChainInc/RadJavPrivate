macro (fixPath path)
	if (${path})
		string (REGEX REPLACE "\\\\" "/" ${path} ${${path}})
	endif ()
endmacro (fixPath)

macro (searchForHeader lib mainHeader searchPath useOnlyParentDir)
	set (${lib}_HEADER_PATHS ${searchPath}/include 
		${searchPath}/Include ${searchPath}/include/${lib} ${searchPath}/Headers
		/usr/local/include /usr/include /opt/local/include /opt/include
		/usr/local/include/${lib} /usr/include/${lib} /opt/local/include/${lib} /opt/include/${lib}
		${searchPath}/build_windows
		${searchPath}/build/include
		${searchPath}/OgreMain/include
		${searchPath}/include/RadJav
		${searchPath}/lib/includes ${searchPath}/lib/includes/${lib}
		/usr/include/leveldb/helpers)

	find_path (${lib}_INCLUDE_DIR NAMES ${mainHeader} HINTS 
		${searchPath} ${${lib}_HEADER_PATHS} PATH_SUFFIXES ${lib})

	if (${useOnlyParentDir})
		get_filename_component (${lib}_INCLUDE_DIR ${${lib}_INCLUDE_DIR} DIRECTORY)
	endif ()

	set (${lib}_INCLUDE ${${lib}_INCLUDE_DIR} ${${lib}_INCLUDE})
endmacro (searchForHeader)

macro (searchForHeader2 lib mainHeader searchPath useOnlyParentDir)
	set (${lib}_HEADER_PATHS ${searchPath}/include 
		${searchPath}/Include ${searchPath}/include/${lib} ${searchPath}/Headers
		/usr/local/include /usr/include /opt/local/include /opt/include
		/usr/local/include/${lib} /usr/include/${lib} /opt/local/include/${lib} /opt/include/${lib}
		${searchPath}/build_windows
		${searchPath}/build/include
		${searchPath}/OgreMain/include
		${searchPath}/lib/includes ${searchPath}/lib/includes/${lib}
		/usr/include/leveldb/helpers)

	find_path (${lib}_INCLUDE_DIR2 NAMES ${mainHeader} HINTS 
		${searchPath} ${${lib}_HEADER_PATHS} PATH_SUFFIXES ${lib})

	if (${useOnlyParentDir})
		get_filename_component (${lib}_INCLUDE_DIR2 ${${lib}_INCLUDE_DIR2} DIRECTORY)
	endif ()

	set (${lib}_INCLUDE ${${lib}_INCLUDE_DIR2} ${${lib}_INCLUDE})
endmacro (searchForHeader2)

macro (searchForLibrary lib debugLibraries releaseLibraries searchPath)
	set (${lib}_LIBRARY_DEBUG_PATHS ${searchPath}/lib 
		${searchPath}/Lib ${searchPath}/lib/${lib} ${searchPath}/Libs
		${searchPath}/lib/debug ${searchPath}/Lib/debug ${searchPath}/lib/${lib}/debug 
		${searchPath}/Libs/debug 
		${searchPath}/lib/vc_lib /usr/local/lib /usr/lib /opt/local/lib /opt/lib 
		/usr/lib/x86_64-linux-gnu 
		${searchPath}/out/arm.debug/obj.host/src 
		${searchPath}/out/arm.debug/obj.host/third_party/icu 
		${searchPath}/gypfiles/Debug/lib 
		${searchPath}/out.gn/arm.debug
		${searchPath}/out.gn/arm.debug/obj 
		${searchPath}/out.gn/arm.debug/obj/third_party/icu 
		${searchPath}/out.gn/arm.release
		${searchPath}/out.gn/arm.release/obj 
		${searchPath}/out.gn/arm.release/obj/third_party/icu 
		${searchPath}/build/Debug/lib ${searchPath}/build/debug 
		${searchPath}/build/debug ${searchPath}/build/Debug 
		${searchPath}/build/lib/Debug 
		${searchPath}/build/src/Debug 
		${searchPath}/msvc/Release 
		${searchPath}/vcbuild/src/Debug 
		${searchPath}/bin 
		${searchPath}/Debug)

	if ((USE_BITS STREQUAL "32") OR (USE_BITS STREQUAL ""))
		set (${lib}_LIBRARY_DEBUG_PATHS 
			${searchPath}/out.gn/ia32.debug 
			${searchPath}/out.gn/ia32.debug/obj 
			${searchPath}/out.gn/ia32.debug/obj/third_party/icu 
			${searchPath}/out.gn/ia32.release 
			${searchPath}/out.gn/ia32.release/obj 
			${searchPath}/out.gn/ia32.release/obj/third_party/icu 
			${searchPath}/buildx86 ${searchPath}/buildx86/Debug/lib ${searchPath}/buildx86/Debug 
			${searchPath}/buildx86/debug ${searchPath}/buildx86/lib/Debug ${searchPath}/buildx86/src/Debug 
			${searchPath}/build-x86 ${searchPath}/build-x86/Debug/lib ${searchPath}/build-x86/Debug 
			${searchPath}/build-x86/debug ${searchPath}/build-x86/lib/Debug ${searchPath}/build-x86/src/Debug 
			${searchPath}/build-32bit ${searchPath}/build-32bit/Debug/lib ${searchPath}/build-32bit/Debug 
			${searchPath}/build-32bit/debug ${searchPath}/build-32bit/lib/Debug ${searchPath}/build-32bit/src/Debug 
			${searchPath}/build32bit ${searchPath}/build32bit/Debug/lib ${searchPath}/build32bit/Debug 
			${searchPath}/build32bit/debug ${searchPath}/build32bit/lib/Debug ${searchPath}/build32bit/src/Debug 
			"${searchPath}/build_windows/Win32/Static Debug" 
			${${lib}_LIBRARY_DEBUG_PATHS})
	endif ()

	if ((USE_BITS STREQUAL "64") OR (USE_BITS STREQUAL ""))
		set (${lib}_LIBRARY_DEBUG_PATHS 
			${searchPath}/out.gn/x64.debug 
			${searchPath}/out.gn/x64.debug/obj 
			${searchPath}/out.gn/x64.debug/obj/third_party/icu 
			${searchPath}/out.gn/x64.release 
			${searchPath}/out.gn/x64.release/obj 
			${searchPath}/out.gn/x64.release/obj/third_party/icu 
			${searchPath}/buildx64 ${searchPath}/buildx64/Debug/lib ${searchPath}/buildx64/Debug 
			${searchPath}/buildx64/debug ${searchPath}/buildx64/lib/Debug ${searchPath}/buildx64/src/Debug 
			${searchPath}/build-x64 ${searchPath}/build-x64/Debug/lib ${searchPath}/build-x64/Debug 
			${searchPath}/build-x64/debug ${searchPath}/build-x64/lib/Debug ${searchPath}/build-x64/src/Debug 
			${searchPath}/build-64bit ${searchPath}/build-64bit/Debug/lib ${searchPath}/build-64bit/Debug 
			${searchPath}/build-64bit/debug ${searchPath}/build-64bit/lib/Debug ${searchPath}/build-64bit/src/Debug 
			${searchPath}/build64bit ${searchPath}/build64bit/Debug/lib ${searchPath}/build64bit/Debug 
			${searchPath}/build64bit/debug ${searchPath}/build64bit/lib/Debug ${searchPath}/build64bit/src/Debug 
			"${searchPath}/build_windows/Win64/Static Debug" 
			${${lib}_LIBRARY_DEBUG_PATHS})
	endif ()

	unset (${lib}_LIBRARY_DEBUG CACHE)
	find_library (${lib}_LIBRARY_DEBUG NAMES ${debugLibraries} HINTS 
		${searchPath} ${${lib}_LIBRARY_DEBUG_PATHS} PATH_SUFFIXES "" 
		debug)

	set (${lib}_LIBRARY_RELEASE_PATHS ${searchPath}/lib 
		${searchPath}/Lib ${searchPath}/lib/${lib} ${searchPath}/Libs 
		${searchPath}/lib/release ${searchPath}/Lib/release ${searchPath}/lib/${lib}/release 
		${searchPath}/Libs/release 
		${searchPath}/lib/vc_lib ${searchPath}/out/arm.release/obj.host/src 
		/usr/lib/x86_64-linux-gnu 
		${searchPath}/out/arm.release/obj.host/third_party/icu 
		${searchPath}/gypfiles/Release/lib 
		${searchPath}/out.gn/arm.release
		${searchPath}/out.gn/arm.release/obj 
		${searchPath}/out.gn/arm.release/obj/third_party/icu 
		${searchPath}/build/Release/lib ${searchPath}/build/release 
		${searchPath}/build/release ${searchPath}/build/Release 
		${searchPath}/build/lib/Release 
		${searchPath}/build/src/Release 
		${searchPath}/msvc/Release 
		${searchPath}/vcbuild/src/Release 
		${searchPath}/bin 
		${searchPath}/Release)

	if ((USE_BITS STREQUAL "32") OR (USE_BITS STREQUAL ""))
		set (${lib}_LIBRARY_RELEASE_PATHS 
			${searchPath}/out.gn/ia32.release
			${searchPath}/out.gn/ia32.release/obj
			${searchPath}/out.gn/ia32.release/obj/third_party/icu
			${searchPath}/buildx86 ${searchPath}/buildx86/Release/lib ${searchPath}/buildx86/Release 
			${searchPath}/buildx86/release ${searchPath}/buildx86/lib/Release ${searchPath}/buildx86/src/Release 
			${searchPath}/build-x86 ${searchPath}/build-x86/Release/lib ${searchPath}/build-x86/Release 
			${searchPath}/build-x86/release ${searchPath}/build-x86/lib/Release ${searchPath}/build-x86/src/Release 
			${searchPath}/build-32bit ${searchPath}/build-32bit/Release/lib ${searchPath}/build-32bit/Release 
			${searchPath}/build-32bit/release ${searchPath}/build-32bit/lib/Release ${searchPath}/build-32bit/src/Release 
			${searchPath}/build32bit ${searchPath}/build32bit/Release/lib ${searchPath}/build32bit/Release 
			${searchPath}/build32bit/release ${searchPath}/build32bit/lib/Release ${searchPath}/build32bit/src/Release 
			"${searchPath}/build_windows/Win32/Static Release" 
			${${lib}_LIBRARY_RELEASE_PATHS})
	endif ()

	if ((USE_BITS STREQUAL "64") OR (USE_BITS STREQUAL ""))
		set (${lib}_LIBRARY_RELEASE_PATHS 
			${searchPath}/out.gn/x64.release
			${searchPath}/out.gn/x64.release/obj 
			${searchPath}/out.gn/x64.release/obj/third_party/icu 
			${searchPath}/buildx64 ${searchPath}/buildx64/Release/lib ${searchPath}/buildx64/Release 
			${searchPath}/buildx64/release ${searchPath}/buildx64/lib/Release ${searchPath}/buildx64/src/Release 
			${searchPath}/build-x64 ${searchPath}/build-x64/Release/lib ${searchPath}/build-x64/Release 
			${searchPath}/build-x64/release ${searchPath}/build-x64/lib/Release ${searchPath}/build-x64/src/Release 
			${searchPath}/build-64bit ${searchPath}/build-64bit/Release/lib ${searchPath}/build-64bit/Release 
			${searchPath}/build-64bit/release ${searchPath}/build-64bit/lib/Release ${searchPath}/build-64bit/src/Release 
			${searchPath}/build64bit ${searchPath}/build64bit/Release/lib ${searchPath}/build64bit/Release 
			${searchPath}/build64bit/release ${searchPath}/build64bit/lib/Release ${searchPath}/build64bit/src/Release 
			"${searchPath}/build_windows/Win64/Static Release" 
			${${lib}_LIBRARY_RELEASE_PATHS})
	endif ()

	unset (${lib}_LIBRARY_RELEASE CACHE)
	find_library (${lib}_LIBRARY_RELEASE NAMES ${releaseLibraries} HINTS 
		${searchPath} ${${lib}_LIBRARY_RELEASE_PATHS} PATH_SUFFIXES "" 
		release relwithdebinfo minsizerel)

	set(${lib}_LIBRARIES debug ${${lib}_LIBRARY_DEBUG} optimized ${${lib}_LIBRARY_RELEASE} ${${lib}_LIBRARIES})
endmacro (searchForLibrary)

macro (hasLibBeenFound lib)
	if (${lib}_INCLUDE)
		if (${lib}_LIBRARY_DEBUG_PATHS)
			if (${lib}_LIBRARY_RELEASE_PATHS)
				set (${lib}_FOUND TRUE)
			endif ()
		endif ()
	endif ()
endmacro (hasLibBeenFound)

macro (hasHeaderBeenFound lib)
	if (${lib}_INCLUDE)
		set (${lib}_FOUND TRUE)
	endif ()
endmacro (hasHeaderBeenFound)

