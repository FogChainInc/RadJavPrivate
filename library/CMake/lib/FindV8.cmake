message (STATUS "Searching for V8...")

set (V8_SOURCE $ENV{V8_SOURCE} CACHE PATH "V8 path")
fixPath (V8_SOURCE)

string(CONCAT icu_path ${V8_SOURCE} "/../third_party/icu/source/common/")

set (V8_SEARCH_PATHS ${V8_SOURCE} ${RADJAV_DEPENDENCIES})

if (V8_SEARCH_PATHS)
	searchForHeader (V8 v8.h ${V8_SEARCH_PATHS} FALSE)
	searchForHeader (ICU util.h ${icu_path} FALSE)
	#searchForLibrary (V8 v8_libbase v8_libbase ${V8_SEARCH_PATHS})

	#if (WIN32)
	#	searchForLibrary (V8 v8_base_0 v8_base_0 ${V8_SEARCH_PATHS})
	#	searchForLibrary (V8 v8_base_1 v8_base_1 ${V8_SEARCH_PATHS})
	#	searchForLibrary (V8 v8_base_2 v8_base_2 ${V8_SEARCH_PATHS})
	#	searchForLibrary (V8 v8_base_3 v8_base_3 ${V8_SEARCH_PATHS})
	#else ()
	#	searchForLibrary (V8 v8_base v8_base ${V8_SEARCH_PATHS})
	#endif()

	#searchForLibrary (V8 v8_nosnapshot v8_nosnapshot ${V8_SEARCH_PATHS})
	#searchForLibrary (V8 v8_libplatform v8_libplatform ${V8_SEARCH_PATHS})

	if (WIN32)
		searchForLibrary (V8 v8.dll v8.dll ${V8_SEARCH_PATHS})
		searchForLibrary (V8 v8_libplatform.dll v8_libplatform.dll ${V8_SEARCH_PATHS})
		searchForLibrary (V8 v8_libbase.dll v8_libbase.dll ${V8_SEARCH_PATHS})
		searchForLibrary (V8 icui18n.dll icui18n.dll ${V8_SEARCH_PATHS})
		searchForLibrary (V8 icuuc.dll icuuc.dll ${V8_SEARCH_PATHS})
	else()
		searchForLibrary (V8 v8 v8 ${V8_SEARCH_PATHS})
		searchForLibrary (V8 v8_libplatform v8_libplatform ${V8_SEARCH_PATHS})
		searchForLibrary (V8 v8_libbase v8_libbase ${V8_SEARCH_PATHS})
		searchForLibrary (V8 icui18n icui18n ${V8_SEARCH_PATHS})
		searchForLibrary (V8 icuuc icuuc ${V8_SEARCH_PATHS})
	endif()
endif ()

if (WIN32)
	set(V8_LIBRARIES debug winmm.lib Dbghelp.lib optimized winmm.lib Dbghelp.lib ${V8_LIBRARIES})
endif ()

hasLibBeenFound (V8)

