/*
 MIT-LICENSE
 Copyright (c) 2018 Higher Edge Software, LLC
 
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

#include "RadJavC.h"
#include "RadJav.h"

#include <iostream>

static int initialize(RadJAV::Array<RadJAV::String> args);

int RadJav_initialize(int argc, const char * argv[])
{
	RadJAV::Array<RadJAV::String> args;
	
	for (int i = 0; i < argc; i++)
		args.push_back(argv[i]);
	
	if (args.size() < 1)
		args.push_back("");
	
	return initialize(args);
}

void RadJav_shutdown(void)
{
	RadJAV::RadJav::shutdown();
}

void RadJav_showError(const char* message)
{
	RadJAV::RadJav::showError(message);
}

int initialize(RadJAV::Array<RadJAV::String> args)
{
	RadJAV::String file = "";
	
	if (RadJAV::RadJav::initialize(args, file) == RadJAV::RadJavType::XRJ_NODE)
		return (EXIT_SUCCESS);
	
	try
	{
		if (args.size() < 2)
		{
			RadJav_showError("No files to execute or arguments specified!");
			
			return (EXIT_FAILURE);
		}
		
		return (RadJAV::RadJav::runApplicationFromFile (file));
	}
	catch (RadJAV::Exception ex)
	{
		RadJav_showError(ex.getMessage().c_str());
		
		return (EXIT_FAILURE);
	}
}
