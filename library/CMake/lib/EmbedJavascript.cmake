
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
		
		#TODO: check 8000 value - here it is possible that we accidentally divide escape
		#sequence and our code will be broken later, it is better to search end of line
		#near the 8000 margin to split strings gracefully
		string (LENGTH "${jsFileContent}" jsFileContentLength)
		
		if (jsFileContentLength GREATER 8000)
			
			#Adding open vector code
			file (APPEND ${headerFilePath} "
			javascriptFiles.push_back (JSFile (\"${_jsFile}\", Array<String>({\"")
				
			while(jsFileContentLength GREATER 8000)
				#for every jsFileContentChunk
				#do string(SUBSTRING <string> <begin> <length> <out-var>)
				
				#get the chunk
				string (SUBSTRING "${jsFileContent}" 0 8000 jsFileContentChunk)
				#replace source
				string (SUBSTRING "${jsFileContent}" 8000 -1 jsFileContent)
				#calculate new length
				string (LENGTH "${jsFileContent}" jsFileContentLength)
				
				file (APPEND ${headerFilePath} "${jsFileContentChunk}")
			
				#add ", " in between
				file (APPEND ${headerFilePath} "\", \"")
			
			endwhile(jsFileContentLength GREATER 8000)
			
			#add last chunk
			string (SUBSTRING "${jsFileContent}" 0 8000 jsFileContentChunk)
			file (APPEND ${headerFilePath} "${jsFileContentChunk}")
			#and finalization
			file (APPEND ${headerFilePath} "\"})));")
		
		else (jsFileContentLength GREATER 8000)
		
			#Adding open vector code
			file (APPEND ${headerFilePath} "
			javascriptFiles.push_back (JSFile (\"${_jsFile}\", \"")

			#Adding content of Javascript file (use chunk here, not the full content
			file (APPEND ${headerFilePath} "${jsFileContent}")
			
			#Adding close vector code
			file (APPEND ${headerFilePath} "\"));")
		
		endif (jsFileContentLength GREATER 8000)
		
		#end for	
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
