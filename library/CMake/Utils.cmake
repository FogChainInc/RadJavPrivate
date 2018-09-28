macro (fixPath path)
	if (${path})
		string (REGEX REPLACE "\\\\" "/" ${path} ${${path}})
	endif ()
endmacro (fixPath)

macro (searchForHeader lib mainHeader searchPath useOnlyParentDir)
	set (${lib}_HEADER_PATHS ${searchPath} ${searchPath}/include 
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
	set (${lib}_HEADER_PATHS ${searchPath} ${searchPath}/include 
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

macro (searchForLibraryOnlyInPath lib debugLibraries releaseLibraries searchPath debugPaths releasePaths)
	find_library (${lib}_LIBRARY_DEBUG NAMES ${debugLibraries} HINTS 
		${searchPath} ${debugPaths} PATH_SUFFIXES "" 
		debug)

	find_library (${lib}_LIBRARY_RELEASE NAMES ${releaseLibraries} HINTS 
		${searchPath} ${releasePaths} PATH_SUFFIXES "" 
		release relwithdebinfo minsizerel)

	set(${lib}_LIBRARIES debug ${${lib}_LIBRARY_DEBUG} optimized ${${lib}_LIBRARY_RELEASE} ${${lib}_LIBRARIES})
endmacro (searchForLibraryOnlyInPath)

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
		${searchPath}/Debug ${searchPath})

	if (USE_ARCH STREQUAL "x86")
		set (${lib}_LIBRARY_DEBUG_PATHS 
			${searchPath}/out.gn/ia32.debug 
			${searchPath}/out.gn/ia32.debug/obj 
			${searchPath}/out.gn/ia32.debug/obj/third_party/icu 
			${searchPath}/out.gn/ia32.release 
			${searchPath}/out.gn/ia32.release/obj 
			${searchPath}/out.gn/ia32.release/obj/third_party/icu 
			${searchPath}/buildx86 ${searchPath}/buildx86/Debug/lib ${searchPath}/buildx86/Debug 
			${searchPath}/buildx86/debug ${searchPath}/buildx86/lib/Debug ${searchPath}/buildx86/src/Debug 
			${searchPath}/buildx86/lib 
			${searchPath}/build-x86 ${searchPath}/build-x86/Debug/lib ${searchPath}/build-x86/Debug 
			${searchPath}/build-x86/debug ${searchPath}/build-x86/lib/Debug ${searchPath}/build-x86/src/Debug 
			${searchPath}/build-x86/lib 
			${searchPath}/build-32bit ${searchPath}/build-32bit/Debug/lib ${searchPath}/build-32bit/Debug 
			${searchPath}/build-32bit/debug ${searchPath}/build-32bit/lib/Debug ${searchPath}/build-32bit/src/Debug 
			${searchPath}/build-32bit/lib 
			${searchPath}/build32bit ${searchPath}/build32bit/Debug/lib ${searchPath}/build32bit/Debug 
			${searchPath}/build32bit/debug ${searchPath}/build32bit/lib/Debug ${searchPath}/build32bit/src/Debug 
			${searchPath}/build32bit/lib 
			"${searchPath}/build_windows/Win32/Static Debug" 
			${searchPath}/x86 
			${searchPath}/stage/x86/lib 
			${${lib}_LIBRARY_DEBUG_PATHS})
	endif ()

	if (USE_ARCH STREQUAL "x86_64")
		set (${lib}_LIBRARY_DEBUG_PATHS 
			${searchPath}/out.gn/x64.debug 
			${searchPath}/out.gn/x64.debug/obj 
			${searchPath}/out.gn/x64.debug/obj/third_party/icu 
			${searchPath}/out.gn/x64.release 
			${searchPath}/out.gn/x64.release/obj 
			${searchPath}/out.gn/x64.release/obj/third_party/icu 
			${searchPath}/buildx64 ${searchPath}/buildx64/Debug/lib ${searchPath}/buildx64/Debug 
			${searchPath}/buildx64/debug ${searchPath}/buildx64/lib/Debug ${searchPath}/buildx64/src/Debug 
			${searchPath}/buildx64/lib 
			${searchPath}/build-x64 ${searchPath}/build-x64/Debug/lib ${searchPath}/build-x64/Debug 
			${searchPath}/build-x64/debug ${searchPath}/build-x64/lib/Debug ${searchPath}/build-x64/src/Debug 
			${searchPath}/build-x64/lib 
			${searchPath}/build-64bit ${searchPath}/build-64bit/Debug/lib ${searchPath}/build-64bit/Debug 
			${searchPath}/build-64bit/debug ${searchPath}/build-64bit/lib/Debug ${searchPath}/build-64bit/src/Debug 
			${searchPath}/build-64bit/lib 
			${searchPath}/build64bit ${searchPath}/build64bit/Debug/lib ${searchPath}/build64bit/Debug 
			${searchPath}/build64bit/debug ${searchPath}/build64bit/lib/Debug ${searchPath}/build64bit/src/Debug 
			${searchPath}/build64bit/lib 
			"${searchPath}/build_windows/Win64/Static Debug" 
			${searchPath}/x64 
			${searchPath}/stage/x64/lib 
			${${lib}_LIBRARY_DEBUG_PATHS})
	endif ()
	#if ((USE_ARCH STREQUAL "armeabi") OR (USE_ARCH STREQUAL "armeabi-v7a") OR (USE_ARCH STREQUAL "arm64-v8a"))
	#if (${USE_ARCH} MATCHES "^arm.*")
	if (USE_ARCH STREQUAL "armeabi")
		set (${lib}_LIBRARY_DEBUG_PATHS 
			${searchPath}/out.gn/armeabi.debug 
			${searchPath}/out.gn/armeabi.debug/obj 
			${searchPath}/out.gn/armeabi.debug/obj/third_party/icu 
			${searchPath}/out.gn/armeabi.release 
			${searchPath}/out.gn/armeabi.release/obj 
			${searchPath}/out.gn/armeabi.release/obj/third_party/icu 
			${searchPath}/buildarmeabi ${searchPath}/buildarmeabi/Debug/lib ${searchPath}/buildarmeabi/Debug 
			${searchPath}/buildarmeabi/debug ${searchPath}/buildarmeabi/lib/Debug ${searchPath}/buildarmeabi/src/Debug 
			${searchPath}/buildarmeabi/lib 
			${searchPath}/build-armeabi ${searchPath}/build-armeabi/Debug/lib ${searchPath}/build-armeabi/Debug 
			${searchPath}/build-armeabi/debug ${searchPath}/build-armeabi/lib/Debug ${searchPath}/build-armeabi/src/Debug 
			${searchPath}/build-armeabi/lib 
			${searchPath}/build-armeabi ${searchPath}/build-armeabi/Debug/lib ${searchPath}/build-armeabi/Debug 
			${searchPath}/build-armeabi/debug ${searchPath}/build-armeabi/lib/Debug ${searchPath}/build-armeabi/src/Debug 
			${searchPath}/build-armeabi/lib 
			${searchPath}/buildarmeabi ${searchPath}/buildarmeabi/Debug/lib ${searchPath}/buildarmeabi/Debug 
			${searchPath}/buildarmeabi/debug ${searchPath}/buildarmeabi/lib/Debug ${searchPath}/buildarmeabi/src/Debug 
			${searchPath}/buildarmeabi/lib 
			"${searchPath}/build_windows/armeabi/Static Debug" 
			${searchPath}/armeabi 
			${searchPath}/stage/armeabi/lib 
			${${lib}_LIBRARY_DEBUG_PATHS})
	endif ()

	if (USE_ARCH STREQUAL "armeabi-v7a")
		set (${lib}_LIBRARY_DEBUG_PATHS 
			${searchPath}/out.gn/armeabi-v7a.debug 
			${searchPath}/out.gn/armeabi-v7a.debug/obj 
			${searchPath}/out.gn/armeabi-v7a.debug/obj/third_party/icu 
			${searchPath}/out.gn/armeabi-v7a.release 
			${searchPath}/out.gn/armeabi-v7a.release/obj 
			${searchPath}/out.gn/armeabi-v7a.release/obj/third_party/icu 
			${searchPath}/buildarmeabi-v7a ${searchPath}/buildarmeabi-v7a/Debug/lib ${searchPath}/buildarmeabi-v7a/Debug 
			${searchPath}/buildarmeabi-v7a/debug ${searchPath}/buildarmeabi-v7a/lib/Debug ${searchPath}/buildarmeabi-v7a/src/Debug 
			${searchPath}/buildarmeabi-v7a/lib 
			${searchPath}/build-armeabi-v7a ${searchPath}/build-armeabi-v7a/Debug/lib ${searchPath}/build-armeabi-v7a/Debug 
			${searchPath}/build-armeabi-v7a/debug ${searchPath}/build-armeabi-v7a/lib/Debug ${searchPath}/build-armeabi-v7a/src/Debug 
			${searchPath}/build-armeabi-v7a/lib 
			${searchPath}/build-armeabi-v7a ${searchPath}/build-armeabi-v7a/Debug/lib ${searchPath}/build-armeabi-v7a/Debug 
			${searchPath}/build-armeabi-v7a/debug ${searchPath}/build-armeabi-v7a/lib/Debug ${searchPath}/build-armeabi-v7a/src/Debug 
			${searchPath}/build-armeabi-v7a/lib 
			${searchPath}/buildarmeabi-v7a ${searchPath}/buildarmeabi-v7a/Debug/lib ${searchPath}/buildarmeabi-v7a/Debug 
			${searchPath}/buildarmeabi-v7a/debug ${searchPath}/buildarmeabi-v7a/lib/Debug ${searchPath}/buildarmeabi-v7a/src/Debug 
			${searchPath}/buildarmeabi-v7a/lib 
			"${searchPath}/build_windows/armeabi-v7a/Static Debug" 
			${searchPath}/armeabi-v7a 
			${searchPath}/stage/armeabi-v7a/lib 
			${${lib}_LIBRARY_DEBUG_PATHS})
	endif ()

	if (USE_ARCH STREQUAL "arm64-v8a")
		set (${lib}_LIBRARY_DEBUG_PATHS 
			${searchPath}/out.gn/arm64-v8a.debug 
			${searchPath}/out.gn/arm64-v8a.debug/obj 
			${searchPath}/out.gn/arm64-v8a.debug/obj/third_party/icu 
			${searchPath}/out.gn/arm64-v8a.release 
			${searchPath}/out.gn/arm64-v8a.release/obj 
			${searchPath}/out.gn/arm64-v8a.release/obj/third_party/icu 
			${searchPath}/buildarm64-v8a ${searchPath}/buildarm64-v8a/Debug/lib ${searchPath}/buildarm64-v8a/Debug 
			${searchPath}/buildarm64-v8a/debug ${searchPath}/buildarm64-v8a/lib/Debug ${searchPath}/buildarm64-v8a/src/Debug 
			${searchPath}/buildarm64-v8a/lib 
			${searchPath}/build-arm64-v8a ${searchPath}/build-arm64-v8a/Debug/lib ${searchPath}/build-arm64-v8a/Debug 
			${searchPath}/build-arm64-v8a/debug ${searchPath}/build-arm64-v8a/lib/Debug ${searchPath}/build-arm64-v8a/src/Debug 
			${searchPath}/build-arm64-v8a/lib 
			${searchPath}/build-arm64-v8a ${searchPath}/build-arm64-v8a/Debug/lib ${searchPath}/build-arm64-v8a/Debug 
			${searchPath}/build-arm64-v8a/debug ${searchPath}/build-arm64-v8a/lib/Debug ${searchPath}/build-arm64-v8a/src/Debug 
			${searchPath}/build-arm64-v8a/lib 
			${searchPath}/buildarm64-v8a ${searchPath}/buildarm64-v8a/Debug/lib ${searchPath}/buildarm64-v8a/Debug 
			${searchPath}/buildarm64-v8a/debug ${searchPath}/buildarm64-v8a/lib/Debug ${searchPath}/buildarm64-v8a/src/Debug 
			${searchPath}/buildarm64-v8a/lib 
			"${searchPath}/build_windows/arm64-v8a/Static Debug" 
			${searchPath}/arm64-v8a 
			${searchPath}/stage/arm64-v8a/lib 
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
		${searchPath}/Release
		${searchPath})

	if (USE_ARCH STREQUAL "x86")
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

	if (USE_ARCH STREQUAL "x86_64")
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

	if (USE_ARCH STREQUAL "armeabi")
		set (${lib}_LIBRARY_RELEASE_PATHS 
			${searchPath}/out.gn/armeabi.release
			${searchPath}/out.gn/armeabi.release/obj
			${searchPath}/out.gn/armeabi.release/obj/third_party/icu
			${searchPath}/buildarmeabi ${searchPath}/buildarmeabi/Release/lib ${searchPath}/buildarmeabi/Release 
			${searchPath}/buildarmeabi/release ${searchPath}/buildarmeabi/lib/Release ${searchPath}/buildarmeabi/src/Release 
			${searchPath}/build-armeabi ${searchPath}/build-armeabi/Release/lib ${searchPath}/build-armeabi/Release 
			${searchPath}/build-armeabi/release ${searchPath}/build-armeabi/lib/Release ${searchPath}/build-armeabi/src/Release 
			${searchPath}/build-armeabi ${searchPath}/build-armeabi/Release/lib ${searchPath}/build-armeabi/Release 
			${searchPath}/build-armeabi/release ${searchPath}/build-armeabi/lib/Release ${searchPath}/build-armeabi/src/Release 
			${searchPath}/buildarmeabi ${searchPath}/buildarmeabi/Release/lib ${searchPath}/buildarmeabi/Release 
			${searchPath}/buildarmeabi/release ${searchPath}/buildarmeabi/lib/Release ${searchPath}/buildarmeabi/src/Release 
			"${searchPath}/build_windows/armeabi/Static Release" 
			${${lib}_LIBRARY_RELEASE_PATHS})
	endif ()

	if (USE_ARCH STREQUAL "armeabi-v7a")
		set (${lib}_LIBRARY_RELEASE_PATHS 
			${searchPath}/out.gn/armeabi-v7a.release
			${searchPath}/out.gn/armeabi-v7a.release/obj 
			${searchPath}/out.gn/armeabi-v7a.release/obj/third_party/icu 
			${searchPath}/buildarmeabi-v7a ${searchPath}/buildarmeabi-v7a/Release/lib ${searchPath}/buildarmeabi-v7a/Release 
			${searchPath}/buildarmeabi-v7a/release ${searchPath}/buildarmeabi-v7a/lib/Release ${searchPath}/buildarmeabi-v7a/src/Release 
			${searchPath}/build-armeabi-v7a ${searchPath}/build-armeabi-v7a/Release/lib ${searchPath}/build-armeabi-v7a/Release 
			${searchPath}/build-armeabi-v7a/release ${searchPath}/build-armeabi-v7a/lib/Release ${searchPath}/build-armeabi-v7a/src/Release 
			${searchPath}/build-armeabi-v7a ${searchPath}/build-armeabi-v7a/Release/lib ${searchPath}/build-armeabi-v7a/Release 
			${searchPath}/build-armeabi-v7a/release ${searchPath}/build-armeabi-v7a/lib/Release ${searchPath}/build-armeabi-v7a/src/Release 
			${searchPath}/buildarmeabi-v7a ${searchPath}/buildarmeabi-v7a/Release/lib ${searchPath}/buildarmeabi-v7a/Release 
			${searchPath}/buildarmeabi-v7a/release ${searchPath}/buildarmeabi-v7a/lib/Release ${searchPath}/buildarmeabi-v7a/src/Release 
			"${searchPath}/build_windows/armeabi-v7a/Static Release" 
			${${lib}_LIBRARY_RELEASE_PATHS})
	endif ()

	if (USE_ARCH STREQUAL "arm64-v8a")
		set (${lib}_LIBRARY_RELEASE_PATHS 
			${searchPath}/out.gn/arm64-v8a.release
			${searchPath}/out.gn/arm64-v8a.release/obj 
			${searchPath}/out.gn/arm64-v8a.release/obj/third_party/icu 
			${searchPath}/buildarm64-v8a ${searchPath}/buildarm64-v8a/Release/lib ${searchPath}/buildarm64-v8a/Release 
			${searchPath}/buildarm64-v8a/release ${searchPath}/buildarm64-v8a/lib/Release ${searchPath}/buildarm64-v8a/src/Release 
			${searchPath}/build-arm64-v8a ${searchPath}/build-arm64-v8a/Release/lib ${searchPath}/build-arm64-v8a/Release 
			${searchPath}/build-arm64-v8a/release ${searchPath}/build-arm64-v8a/lib/Release ${searchPath}/build-arm64-v8a/src/Release 
			${searchPath}/build-arm64-v8a ${searchPath}/build-arm64-v8a/Release/lib ${searchPath}/build-arm64-v8a/Release 
			${searchPath}/build-arm64-v8a/release ${searchPath}/build-arm64-v8a/lib/Release ${searchPath}/build-arm64-v8a/src/Release 
			${searchPath}/buildarm64-v8a ${searchPath}/buildarm64-v8a/Release/lib ${searchPath}/buildarm64-v8a/Release 
			${searchPath}/buildarm64-v8a/release ${searchPath}/buildarm64-v8a/lib/Release ${searchPath}/buildarm64-v8a/src/Release 
			"${searchPath}/build_windows/arm64-v8a/Static Release" 
			${${lib}_LIBRARY_RELEASE_PATHS})
	endif ()

	if (${USE_ARCH} MATCHES "^arm.*")
		set (${lib}_LIBRARY_RELEASE_PATHS 
			${searchPath}/out.gn/arm64.debug 
			${searchPath}/out.gn/arm64.debug/obj 
			${searchPath}/out.gn/arm64.debug/obj/third_party/icu 
			${searchPath}/out.gn/arm64.release 
			${searchPath}/out.gn/arm64.release/obj 
			${searchPath}/out.gn/arm64.release/obj/third_party/icu 
			${${lib}_LIBRARY_DEBUG_PATHS})
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

