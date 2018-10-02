if (IOS)
set (SOURCES_files_RadJavVM__Resources
	"${RadJavVM_SOURCE_DIR}/res/LaunchScreen.storyboard")
endif ()
if (!IOS)
set (SOURCES_files_RadJavVM__Resources
	"${RadJavVM_SOURCE_DIR}/res/RadJavVM.rc")
endif ()

source_group ("RadJavVM\\Resources" FILES ${SOURCES_files_RadJavVM__Resources})

set (RadJavVM_SOURCES ${SOURCES_files_RadJavVM__Resources} ${RadJavVM_SOURCES})
