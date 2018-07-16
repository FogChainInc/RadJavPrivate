/*
MIT-LICENSE
Copyright (c) 2017 Higher Edge Software, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::string findNewLine = "";
std::string newLine = "";

std::vector<std::string> getTextFromFile(std::string path)
{
	std::vector<std::string> texts;

	std::ifstream file;
	file.open(path.c_str(), std::ios_base::in);

	if (file.is_open() == false)
		return (texts);

	std::cout << "Embedding file: " << path << "\n";
	std::string text = "";
	unsigned long counter = 0;

	while (file.good() == true)
	{
		char cChar = file.get();

		if (file.good() == false)
			break;

		if (counter >= 9000)
		{
			texts.push_back(text);
			counter = 0;
			text = "";
		}

		text += cChar;
		counter++;
	}

	texts.push_back(text);

	return (texts);
}

bool writeToFile(std::string path, std::string text)
{
	std::ofstream file;
	file.open(path.c_str());

	if (file.is_open() == false)
		return (false);

	file << text;
	file.close();

	return (true);
}

std::string replaceText(std::string text, std::string find, std::string replace, int *countNumReplaces = NULL)
{
	unsigned int pos = text.find(find);

	while ((pos != std::string::npos) && (pos < text.size()))
	{
		text.replace(pos, find.size(), replace);
		pos = text.find(find, (pos + replace.size()));

		if (countNumReplaces != NULL)
			(*countNumReplaces)++;
	}

	return (text);
}

std::string fixText(std::string text, int *countNewLines)
{
	text = replaceText(text, "\\n", "\\\\n");
	// Replace all new lines in the javascript files with the appropriate \\n\\ to be
	// used in a large string for the header file.
	text = replaceText(text, findNewLine, "\\n\\" + newLine, countNewLines);
	text = replaceText(text, "\\\"", "\\\\\"");
	text = replaceText(text, "\"", "\\\"");

	return (text);
}

int main(int iArgs, char **cArgs)
{
	std::vector<std::string> args;
	bool pureJavascriptOnly = false;

#if defined (WIN32)
	findNewLine = "\n";
	newLine = "\n";
#elif defined (__APPLE__)
	findNewLine = "\n";
	newLine = "\n";
#else
	findNewLine = "\n";
	newLine = "\n";
#endif

	for (int iIdx = 1; iIdx < iArgs; iIdx++)
	{
		std::string arg = cArgs[iIdx];

		if (arg == "pure-javascript-only")
			pureJavascriptOnly = true;

		args.push_back(arg);
	}

	std::string jsdir = "../javascript/";
	std::string outdir = "../include/RadJav/";
	std::vector<std::string> ary;

	ary.push_back("Math.js");
	ary.push_back("String.js");
	ary.push_back("RadJav.js");
	ary.push_back("RadJav.Color.js");
	ary.push_back("RadJav.Quaternion.js");
	ary.push_back("RadJav.Vector2.js");
	ary.push_back("RadJav.Vector3.js");
	ary.push_back("RadJav.Vector4.js");
	ary.push_back("RadJav.Circle.js");
	ary.push_back("RadJav.Rectangle.js");
	ary.push_back("RadJav.Font.js");
	ary.push_back("RadJav.Thread.js");
	ary.push_back("RadJav.IO.js");
	ary.push_back("RadJav.Interact.js");
	ary.push_back("RadJav.DB.KeyValueStorage.js");
	ary.push_back("RadJav.Crypto.Cipher.js");
	ary.push_back("RadJav.Crypto.Decipher.js");
	ary.push_back("RadJav.Crypto.CipherMultipart.js");
	ary.push_back("RadJav.Crypto.DecipherMultipart.js");
	ary.push_back("RadJav.Crypto.KeyGenerator.js");
	ary.push_back("RadJav.Crypto.PrivateKey.js");
	ary.push_back("RadJav.Crypto.PublicKey.js");
	ary.push_back("RadJav.Crypto.Hash.js");
	ary.push_back("RadJav.Crypto.HashMultipart.js");
	ary.push_back("RadJav.GUI.GObject.js"); // This must be compiled before any other GUI object.
	ary.push_back("RadJav.GUI.Button.js");
	ary.push_back("RadJav.GUI.Canvas3D.js");
	ary.push_back("RadJav.GUI.Checkbox.js");
	ary.push_back("RadJav.GUI.Combobox.js");
	ary.push_back("RadJav.GUI.Container.js");
	ary.push_back("RadJav.GUI.Image.js");
	ary.push_back("RadJav.GUI.Label.js");
	ary.push_back("RadJav.GUI.List.js");
	ary.push_back("RadJav.GUI.Radio.js");
	ary.push_back("RadJav.GUI.Textarea.js");
	ary.push_back("RadJav.GUI.Textbox.js");
	ary.push_back("RadJav.GUI.WebView.js");
	ary.push_back("RadJav.GUI.Window.js");
	ary.push_back("RadJav.GUI.MenuBar.js");
	ary.push_back("RadJav.GUI.MenuItem.js");
	ary.push_back("RadJav.GUI.KeyEvent.js");
	ary.push_back("RadJav.GUI.MouseEvent.js");
	ary.push_back("RadJav.Net.WebSocketServer.js");
	ary.push_back("RadJav.Net.WebSocketClient.js");
	ary.push_back("RadJav.Net.WebServer.js");
#ifdef C3D_USE_OGRE
	ary.push_back("RadJav.GUI.Canvas3D.js");
	ary.push_back("RadJav.C3D.Object3D.js"); // This must be compiled before any other C3D object.
	ary.push_back("RadJav.C3D.Entity.js");
	ary.push_back("RadJav.C3D.Camera.js");
	ary.push_back("RadJav.C3D.Material.js");
	ary.push_back("RadJav.C3D.Model.js");
	ary.push_back("RadJav.C3D.Transform.js");
	ary.push_back("RadJav.C3D.World.js");
	ary.push_back("RadJav.C3D.Light.js");
#endif

	std::string output = "\
#ifndef _RADJAV_JAVASCRIPTCODE_H_" + newLine + "\
	#define _RADJAV_JAVASCRIPTCODE_H_" + newLine + "\
" + newLine + "\
	#include \"RadJavArray.h\"" + newLine + "\
	#include \"RadJavString.h\"" + newLine + "\
	#include \"RadJavHashMap.h\"" + newLine + "\
	#include \"RadJavJSFile.h\"" + newLine + "\
" + newLine + "\
	namespace RadJAV" + newLine + "\
	{" + newLine + "\
		Array<JSFile> javascriptFiles;" + newLine + "\
" + newLine + "\
		inline void loadJavascriptLibrary ()" + newLine + "\
		{" + newLine;
	std::string code = "";
	unsigned int lineCounter = 0;

	for (unsigned int iIdx = 0; iIdx < ary.size(); iIdx++)
	{
		std::string fileName = ary.at(iIdx);
		std::vector<std::string> texts = getTextFromFile(jsdir + fileName);
		std::string textOutput = "";

		if (texts.size() > 1)
			textOutput = "Array<String>({";

		for (unsigned int iJdx = 0; iJdx < texts.size(); iJdx++)
		{
			std::string text = texts.at(iJdx);

			if (text == "")
			{
				std::cout << "Unable to open file" << jsdir << fileName << "\n";

				return (1);
			}

			int pos = text.find("MIT-LICENSE");

			if (pos != std::string::npos)
			{
				pos = text.find("*/");

				if (pos != std::string::npos)
					text = text.substr(pos + 2);
			}

			int countNewLines = 0;

			if (pureJavascriptOnly == false)
			{
				text = fixText(text, &countNewLines);
				lineCounter += countNewLines;
			}

			if (texts.size() > 1)
			{
				if (iJdx == (texts.size() - 1))
				{
					if (fileName == "RadJav.js")
					{
						std::string tempText = "";

#if defined (WIN32)
						tempText += "RadJav.OS.type = \"windows\";" + newLine + "\
RadJav.OS.Windows = function()" + newLine + "\
{" + newLine + "\
}" + newLine;
#elif defined (__APPLE__)
						tempText += "RadJav.OS.type = \"mac\";" + newLine + "\
RadJav.OS.Mac = function()" + newLine + "\
{" + newLine + "\
}" + newLine;
#elif defined (LINUX)
						tempText += "RadJav.OS.type = \"linux\";" + newLine + "\
RadJav.OS.Linux = function()" + newLine + "\
{" + newLine + "\
}" + newLine;
#else
						tempText += "RadJav.OS.type = \"unknown\";" + newLine;
#endif

						if (pureJavascriptOnly == false)
						{
							text += fixText(tempText, &countNewLines);
							lineCounter += countNewLines;
						}
					}
				}
			}

			if (pureJavascriptOnly == true)
				textOutput += text;
			else
				textOutput += "\"" + text + "\"";

			if (texts.size() > 1)
			{
				if (iJdx < (texts.size() - 1))
					textOutput += ", ";
			}
		}

		if (texts.size() > 1)
			textOutput += "})";

		if (pureJavascriptOnly == false)
		{
			code += "\
			javascriptFiles.push_back (JSFile (\"" + fileName + "\", " + textOutput + "));" + newLine;
		}
		else
			code += textOutput;
	}

	output += code;
	output += "" + newLine + "\
		}" + newLine + "\
	}" + newLine + "\
#endif" + newLine + "\
" + newLine;

	if (pureJavascriptOnly == true)
		writeToFile(outdir + "RadJavJavascriptCode.h", code);
	else
		writeToFile(outdir + "RadJavJavascriptCode.h", output);

	std::cout << "Finished embedding javascript\n";

	return (0);
}
