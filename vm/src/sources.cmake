set (SOURCES_files_RadJavVM__Main__Sources
	"${RadJavVM_SOURCE_DIR}/src/RadJavVM/RadJavVM.cpp")
source_group ("RadJavVM\\Main" FILES ${SOURCES_files_RadJavVM__Main__Sources})

set (RadJavVM_SOURCES ${SOURCES_files_RadJavVM__Main__Sources} ${RadJavVM_SOURCES})

set (SOURCES_files_RadJavVMLauncher__Main__Sources
	"${RadJavVM_SOURCE_DIR}/src/RadJavVMLauncher/RadJavVMLauncher.m")
source_group ("RadJavVMLauncher\\Main" FILES ${SOURCES_files_RadJavVMLauncher__Main__Sources})

set (RadJavVMLauncher_SOURCES ${SOURCES_files_RadJavVMLauncher__Main__Sources} ${RadJavVMLauncher_SOURCES})
