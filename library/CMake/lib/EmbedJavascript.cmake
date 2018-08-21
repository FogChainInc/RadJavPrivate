
macro (embedJavascript jsFilesList jsFilesPath headerName headerPath)
	
	set (headerFilePath "${headerPath}/${headerName}")
	message (STATUS "Embedding Javascript into ${headerFilePath}")
	
	#Header start
	set (jsFileContent 
	"#ifndef _RADJAV_JAVASCRIPTCODE_H_
	#define _RADJAV_JAVASCRIPTCODE_H_

	#include \"RadJavArray.h\"
	#include \"RadJavString.h\"
	#include \"RadJavHashMap.h\"
	#include \"RadJavJSFile.h\"

	namespace RadJAV
	{
		Array<JSFile> javascriptFiles\;

		inline void loadJavascriptLibrary ()
		{"
	)
	
	file (WRITE ${headerFilePath} ${jsFileContent})
	
	foreach (_jsFile ${jsFilesList})
		message (STATUS "Embedding ${_jsFile}")
		
		#Read js file content
		file (READ ${jsFilesPath}/${_jsFile} jsFileContent)
		
		#If file is RadJav.js need to add prototype RadJav.OS.type
		if ( ${_jsFile} STREQUAL "RadJav.js")
			addOsTypePrototype(jsFileContent "${jsFileContent}")
		endif ()

		#Removing License comment
		string (REGEX REPLACE "/\\*.*SOFTWARE\\.(\\\n)?\\*/" "" jsFileContent "${jsFileContent}")
		#string (REGEX REPLACE "/\\*.*MIT-LICENSE.*SOFTWARE\\.\n\\*/" "" jsFileContent "${jsFileContent}")
		
		#Escape \n text
		string (REGEX REPLACE "([\\])(n)" "\\1\\1\\2" jsFileContent "${jsFileContent}")

		#Escape \\"
		string (REGEX REPLACE "\\\\\\\"" "\\\\\\\\\"" jsFileContent "${jsFileContent}")

		#Escape "
		string (REGEX REPLACE "\"" "\\\\\"" jsFileContent "${jsFileContent}")

		#Escape/Replace \r\n control characters
		string (REGEX REPLACE "\r?\n" "\\\\n\\\\\n" jsFileContent "${jsFileContent}")
		
		#Adding open vector code
		file (APPEND ${headerFilePath} "
			javascriptFiles.push_back (JSFile (\"${_jsFile}\", \"")

		#Adding content of Javascript file
		file (APPEND ${headerFilePath} "${jsFileContent}")
		
		#Adding close vector code
		file (APPEND ${headerFilePath} "\"));")
		
	endforeach (_jsFile)

	#Header end
	set (jsFileContent
	"		}
	}
#endif"
	)
	
	file (APPEND ${headerFilePath} ${jsFileContent})
	

endmacro (embedJavascript)

function (addOsTypePrototype out headerFileContent)
	if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
		set (_osType "Windows")
	elseif ((${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
		set (_osType "Linux")
	elseif ((${CMAKE_SYSTEM_NAME} STREQUAL "Darwin"))
		set (_osType "Mac")
	elseif ((${CMAKE_SYSTEM_NAME} STREQUAL "Android"))
		set (_osType "Android")
	elseif ((${CMAKE_SYSTEM_NAME} STREQUAL "iOS"))
		set (_osType "iOS")
	else ()
		set (_osType "unknown")
	endif ()
	
	string (TOLOWER ${_osType} _osTypeString)
	
	set (_osTypePrototype "RadJav.OS.type = \"${_osTypeString}\";\n\
RadJav.OS.${_osType} = function()\n\
{\n\
}\n\
")
	
	string (CONCAT headerFileContent "${headerFileContent}" "${_osTypePrototype}")

	set (${out} ${headerFileContent} )
	set (${out} ${${out}} PARENT_SCOPE)

endfunction (addOsTypePrototype)
