var webServer = new RadJav.Net.WebServer ();

webServer.serve (function (request)
	{
		var response = "";

		if (request.type == RadJav.Net.WebServerRequestType.GET)
		{
			if (request.page == "/")
				response = "Hello world!";
		}

		return (response);
	});

webServer.listen (80);

webServer.stop();

var webServer2 = new RadJav.Net.WebServer ();

webServer2.serve (function (request)
	{
		var response = "";

		if (request.type == RadJav.Net.WebServerRequestType.GET)
		{
			if (request.page == "/")
				response = "Hello world!";
		}

		return (response);
	});

webServer2.listen (82);


webServer2.stop();