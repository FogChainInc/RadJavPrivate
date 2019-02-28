message (STATUS "Searching for wxWidgets...")

set (wxWidgets_ROOT_DIR $ENV{WXWIN} CACHE PATH "wxWidgets path")
fixPath (wxWidgets_ROOT_DIR)

#include (FindwxWidgets)

set (wxWidgets_CONFIGURATION ${wxWidgets_CONFIGURATION})
find_package (wxWidgets REQUIRED core base webview)
include (${wxWidgets_USE_FILE})

set (wxWidgets_INCLUDE ${wxWidgets_INCLUDE_DIRS})

if (wxWidgets_USE_DEBUG)
	set (wxWidgets_LIBRARIES debug ${WX_webview} ${wxWidgets_LIBRARIES})
else ()
	set (wxWidgets_LIBRARIES optimized ${WX_webview} ${wxWidgets_LIBRARIES})
endif ()

if (WIN32)
	set(wxWidgets_LIBRARIES debug
		"comctl32.lib;rpcrt4.lib;wsock32.lib;oleacc.lib;wininet.lib;winmm.lib;shlwapi.lib;version.lib"
		optimized "comctl32.lib;rpcrt4.lib;wsock32.lib;oleacc.lib;wininet.lib;winmm.lib;shlwapi.lib;version.lib"
		${wxWidgets_LIBRARIES})
endif ()

hasLibBeenFound (wxWidgets)
