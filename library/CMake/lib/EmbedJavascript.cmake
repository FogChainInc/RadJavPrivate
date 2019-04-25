
macro (embedJavascript jsFilesList jsFilesPath headerFilePath)
	
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
	
	#If .js file will be have more characters than described in value below, it will be splitted
	set (jsFileChunkSize 8000)
	
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
		
		string (LENGTH "${jsFileContent}" jsFileContentLength)
		
		if (jsFileContentLength GREATER ${jsFileChunkSize})
			
			#Adding open vector code
			file (APPEND ${headerFilePath} "
			javascriptFiles.push_back (JSFile (\"${_jsFile}\", Array<String>({R\"jsjsjs(")
				
			while(jsFileContentLength GREATER ${jsFileChunkSize})
				#for every jsFileContentChunk
				#do string(SUBSTRING <string> <begin> <length> <out-var>)
				
				#get the chunk
				string (SUBSTRING "${jsFileContent}" 0 ${jsFileChunkSize} jsFileContentChunk)
				
				#replace source
				string (SUBSTRING "${jsFileContent}" ${jsFileChunkSize} -1 jsFileContent)

				#Get last character from splitted file content
				math(EXPR index ${jsFileChunkSize}-1)
				string (SUBSTRING "${jsFileContentChunk}" ${index} 1 lastCharacter)
				
				#Fixing escaped sequence if any
				if (lastCharacter STREQUAL "\\")
					#Move first character from rest of file content to chunk content so escape sequence will be preserved
					string (SUBSTRING "${jsFileContent}" 0 1 firstCharacter)
					string (CONCAT jsFileContentChunk "${jsFileContentChunk}" "${firstCharacter}")
					string (SUBSTRING "${jsFileContent}" 1 -1 jsFileContent)
				endif ()
				
				#calculate new length
				string (LENGTH "${jsFileContent}" jsFileContentLength)
				
				file (APPEND ${headerFilePath} "${jsFileContentChunk}")
			
				#add ", " in between
				file (APPEND ${headerFilePath} ")jsjsjs\", R\"jsjsjs(")
			
			endwhile()
			
			#add last chunk
			string (SUBSTRING "${jsFileContent}" 0 ${jsFileChunkSize} jsFileContentChunk)
			file (APPEND ${headerFilePath} "${jsFileContentChunk}")
			#and finalization
			file (APPEND ${headerFilePath} ")jsjsjs\"})));")
		
		else ()
		
			#Adding open vector code
			file (APPEND ${headerFilePath} "
			javascriptFiles.push_back (JSFile (\"${_jsFile}\", R\"jsjsjs(")

			#Adding content of Javascript file (use chunk here, not the full content
			file (APPEND ${headerFilePath} "${jsFileContent}")
			
			#Adding close vector code
			file (APPEND ${headerFilePath} ")jsjsjs\"));")
		
		endif ()
		
		#end for	
	endforeach ()

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
	
	set (_osTypePrototype "RadJav.OS.type = \"${_osTypeString}\";
RadJav.OS.${_osType} = function()
{
}

RadJav.OS.HTML5 = null;
")
	
	string (REPLACE "RadJav.GENERATORS_INJECT_RADJAV_OS_CODE_HERE;" "${_osTypePrototype}" headerFileContent "${headerFileContent}")
	string (CONCAT headerFileContent "${headerFileContent}")

	set (${out} ${headerFileContent})
	set (${out} ${${out}} PARENT_SCOPE)

endfunction (addOsTypePrototype)
