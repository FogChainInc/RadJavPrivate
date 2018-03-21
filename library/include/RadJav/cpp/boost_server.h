#pragma once

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			class BoostServer
			{
			public:
				BoostServer(int port, int type);

				/// Listen for any incoming connections.
				void listen();

				/// Serve any GET or POST requests.
				void serve();

				/// Stop the web server.
				void stop();
			};
		}
	}
}