/*
	MIT-LICENSE
	Copyright (c) 2019 FogChain, Corp

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
#include "v8/RadJavV8Inspector.h"

#include "RadJav.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPOS.h"
#include "cpp/RadJavCPPNetWebServer.h"
#include "cpp/RadJavCPPNetWebSocketConnection.h"

#include <boost/algorithm/string/find.hpp>


#ifdef USE_V8
namespace RadJAV
{
	namespace
	{
		std::unique_ptr<uint16_t[]> createMessageBuffer_uint16(v8::Isolate* isolate, String message)
		{
			/*std::unique_ptr<uint16_t[]> msgBuffer(RJNEW uint16_t[message.size()]);
			 v8::Local<v8::String> v8str = message.toV8String(isolate);
			 v8str->Write(isolate, msgBuffer.get(), 0, message.size());
			 
			 v8_inspector::StringView msg(msgBuffer.get(), message.size());*/
			
			std::unique_ptr<uint16_t[]> msgBuffer(RJNEW uint16_t[message.size()]);
			v8::Local<v8::String> v8str = message.toV8String(isolate);
			v8str->Write(isolate, msgBuffer.get(), 0, message.size());
			
			return (msgBuffer);
		}
		
		void networkThreadFunc(CPP::Net::NetworkManager* networkManager)
		{
			if (networkManager)
			{
				// Run network processing here until there is
				// nothing to do (no active contexts with pending operations)
				while(networkManager->run_one())
				{
					// Just make it not eat too much of CPU
					CPP::OS::sleep(1);
				}
			}
		}
	}
	
	V8Inspector::V8Inspector(v8::Isolate *isolate, v8::Local<v8::Context> context)
	{
		isPaused = false;
		server = NULL;
		this->isolate = isolate;
		this->context = context;
		inspector = NULL;
		channel = NULL;
		ready = false;
		webSocketConnection = nullptr;
	}

	V8Inspector::~V8Inspector()
	{
		close();

		// Wait until all network part has nothing to do (WebServer etc.)
		networkThread.join();
	}

	void V8Inspector::start(String ip, RJINT port)
	{
		if (server)
			return;
		
		session.reset();
		inspector = v8_inspector::V8Inspector::create (isolate, this);
		// Seems that we didn't refer to it
		//context->SetAlignedPointerInEmbedderData(kInspectorClientIndex, this);
		inspector->contextCreated(v8_inspector::V8ContextInfo(context, 1, v8_inspector::StringView()));

		server = RJNEW CPP::Net::WebServer(3, networkManager);
		server->onProcess(std::bind(&V8Inspector::onServerProcessCallback, this,
									std::placeholders::_1,
									std::placeholders::_2));
		
		server->onUpgrade(std::bind(&V8Inspector::onServerUpgradeCallback, this,
									std::placeholders::_1,
									std::placeholders::_2));
		
		server->onError(std::bind(&V8Inspector::onServerErrorCallback, this,
								  std::placeholders::_1,
								  std::placeholders::_2));
		
		server->onStop(std::bind(&V8Inspector::onServerStopCallback, this));
		
		// Start the Web server on specific address and port
		server->start(ip, port);

		// Start execution of network part separate from main one
		// Incoming messages will be posted to frontendPendingMessages queue
		networkThread = std::thread(networkThreadFunc, &networkManager);
	}

	void V8Inspector::close()
	{
		session.reset();
		DELETEOBJ(channel);
		server->stop();
		
		if (webSocketConnection)
			webSocketConnection->close();
		
		ready = false;
	}

	void V8Inspector::waitForConnection()
	{
		// Processing all control messages from CDT(Frontend) before compiling/executing any scripts
		// We will be notified when we can start compiling/executing scripts by runIfWaitingForDebugger callback
		while(!ready)
		{
			// Wait for more CDT(Frontend) messages here if any
			CPP::OS::sleep(10);
			dispatchFrontendMessages();
		}

		// Unlock main thread to continue application execution
	}

	void V8Inspector::pauseOnNextStatement()
	{
		String message = "Pause on statement";
		std::unique_ptr<uint16_t[]> msgBuffer = createMessageBuffer_uint16(isolate, message);
		v8_inspector::StringView msg(msgBuffer.get(), message.size());
		
		session->schedulePauseOnNextStatement(msg, msg);
	}

	void V8Inspector::dispatchFrontendMessages()
	{
		while (!frontendPendingMessages.empty())
		{
			const String& message = frontendPendingMessages.front();

			//std::cout << "- Processing CDT message:\n" << message << std::endl;

			std::unique_ptr<uint16_t[]> msgBuffer = createMessageBuffer_uint16 (isolate, message);
			v8_inspector::StringView msg(msgBuffer.get(), message.size());

			session->dispatchProtocolMessage(msg);

			frontendPendingMessages.pop();
		}
	}

	void V8Inspector::onServerProcessCallback(const http::request<http::string_body>& request, http::response<http::string_body>& response)
	{
		using namespace boost::beast;
		
		// Prepare a bad request response
		auto const badRequestResponse = [&request](boost::string_view reason) {
			http::response<http::string_body> res{ http::status::bad_request, request.version() };
			res.set(http::field::content_type, "text/html");
			res.keep_alive(false);
			
			std::ostringstream body;
			body << "<html><head><title>400 Bad Request</title></head><body bgcolor=\"white\"><center><h1>";
			body << reason;
			body << "</h1></center></body></html>";
			res.body() =  body.str();
			
			res.prepare_payload();
			return res;
		};

		// Case insensitive find
		auto icontains = [](const std::string& haystack, const std::string& needle) -> bool {
			boost::iterator_range<std::string::const_iterator> irange;
			irange = boost::ifind_first(haystack, needle);
			return irange;
		};

		// Make sure we can handle the method
		if ((request.method() != http::verb::get) ||
			( !icontains(request.base().target().to_string(), "/json") )   )
		{
			response = badRequestResponse("not a debug session");
			return;
		}
		
		//prepare OK response template
		http::response<http::string_body> res{ http::status::ok, 11 };
		//TODO: check charset settings
		//res.set(http::field::content_type, "application/json; charset=UTF-8");
		//res.set(http::field::accept_charset, "UTF-8");
		res.set(http::field::content_type, "application/json; charset=UTF-8");
		res.set(http::field::cache_control, "no-cache");
		res.keep_alive(false);
		
		String payload;
		
		do //switch for request type
		{
			std::string tempstr = request.base().target().to_string();
			auto str = request.base().target().to_string().substr(strlen("/json"), std::string::npos);
			
			if ((str.length() == 0) // here /json equals to /json/list in http request
				|| icontains(str, "/list"))
			{
				//send.prepareListPayload(&payload);
				payload.append("[ {"); payload.append("\n");
				payload.append("  \"description\": \"v8inspector instance\","); payload.append("\n");
				payload.append("  \"devtoolsFrontendUrl\": \"chrome-devtools://devtools/bundled/inspector.html?"
							   "experiments=true&v8only=true&ws=127.0.0.1:9229/003000e9-00ee-406d-80ac-008100d3000e\","); payload.append("\r\n");
				payload.append("  \"id\": \"003000e9-00ee-406d-80ac-008100d3000e\","); payload.append("\n");
				payload.append("  \"title\": \"RadJav\","); payload.append("\n");
				payload.append("  \"type\": \"node\","); payload.append("\n");
				payload.append("  \"url\": \"file://\","); payload.append("\n");
				payload.append("  \"webSocketDebuggerUrl\": \"ws://127.0.0.1:9229/003000e9-00ee-406d-80ac-008100d3000e\""); payload.append("\n");
				payload.append("} ]"); payload.append("\n\n");
				break;
			}
			if (icontains(str, "/protocol"))
			{
				break;
			}
			if (icontains(str, "/version"))
			{
				payload.append("[ {"); payload.append("\r\n");
				payload.append("  \"Browser\": \"v8inspector\","); payload.append("\r\n");
				payload.append("  \"Protocol-Version\": \"1.3\","); payload.append("\r\n");
				payload.append("] }"); payload.append("\r\n");
				break;
			}
			if (icontains(str, "/activate"))
			{
				break;
			}
		} while (false);
		
		if (!payload.empty())
		{
			res.body() = payload;
			response = res;
			
			response.prepare_payload();
		}
		else
		{
			response = badRequestResponse("target " + request.base().target().to_string() + " is not supported");
		}
	}

	void V8Inspector::onServerUpgradeCallback(CPP::Net::HttpConnection& connection,
											  const http::request<http::string_body>& request)
	{
		//std::cout << "Got WebSocket upgrade request\n";

		//Checking target path
		const boost::string_view acceptableTarget = "/003000e9-00ee-406d-80ac-008100d3000e";
		if (acceptableTarget == request.base().target())
		{
			// Check if we already have WebSocket connection
			if (webSocketConnection)
			{
				// No need to do anything here,
				// WebServer will respond with 400 error if we will not handle upgrade request
				//std::cout << "Not upgrading HttpSocket to WebSocket, already have one\n";
				return;
			}

			// Need to upgrade the socket
			//std::cout << "Upgrading connection to WebSocket\n";
			
			webSocketConnection = CPP::Net::WebSocketConnection::handleUpgrade(&connection, request);
			if (webSocketConnection)
			{
				webSocketConnection->onOpen(std::bind(&V8Inspector::onWebSocketOpenCallback, this));
				webSocketConnection->onMessage(std::bind(&V8Inspector::onWebSocketMessageCallback, this,
														 std::placeholders::_1),
											   std::bind(&V8Inspector::onWebSocketMessageBinaryCallback, this,
														 std::placeholders::_1,
														 std::placeholders::_2));
				
				webSocketConnection->onError(std::bind(&V8Inspector::onWebSocketErrorCallback, this,
													   std::placeholders::_1,
													   std::placeholders::_2));
				
				webSocketConnection->onClose(std::bind(&V8Inspector::onWebSocketCloseCallback, this));
				
				webSocketConnection->setAutoFragmentMessages(false);
			}
		}

		// No need to do anything here,
		// WebServer will respond with 400 error if we will not handle upgrade request
	}
	
	void V8Inspector::onServerStopCallback()
	{
		DELETEOBJ(server);
	}
	
	void V8Inspector::onServerErrorCallback(int errorCode, const std::string& description)
	{
		DELETEOBJ(server);
	}

	void V8Inspector::onWebSocketOpenCallback()
	{
		//std::cout << "Http connection upgraded to WebSocket\n";

		channel = RJNEW V8InspectorChannel(*webSocketConnection);
		session = inspector->connect(1, channel, v8_inspector::StringView());
	}
	
	void V8Inspector::onWebSocketMessageCallback(const std::string& data)
	{
		//std::cout << "Got message from frontend:\n" << data<<std::endl;
		
		// We just put pending frontend(CDT) message into a thread safe queue for future processing
		frontendPendingMessages.push(data);
	}
	
	void V8Inspector::onWebSocketMessageBinaryCallback(const unsigned char* data, std::size_t size)
	{
		//std::cout << "Got binary message from frontend\n";
	}
	
	void V8Inspector::onWebSocketErrorCallback(int ec, const std::string& description)
	{
		//std::cout << "WebSocket error: "<<description<<std::endl;
		
		isPaused = false;
		session.reset();
		
		DELETEOBJ(webSocketConnection);
	}
	
	void V8Inspector::onWebSocketCloseCallback()
	{
		//std::cout << "WebSocket closed\n";
		
		isPaused = false;
		session.reset();
		
		DELETEOBJ(webSocketConnection);
	}

	void V8Inspector::runMessageLoopOnPause(int contextGroupId)
	{
		//std::cout << "- runMessageLoopOnPause\n";
		isPaused = true;

		while (isPaused == true)
		{
			CPP::OS::sleep(10);
			dispatchFrontendMessages();
		}
	}
	
	void V8Inspector::quitMessageLoopOnPause()
	{
		// std::cout << "- quitMessageLoopOnPause\n";
		isPaused = false;
	}
	
	void V8Inspector::runIfWaitingForDebugger(int contextGroupId)
	{
		// std::cout << "- runIfWaitingForDebugger\n";
		ready = true;
	}
	
	void V8Inspector::muteMetrics(int contextGroupId)
	{
	}
	
	void V8Inspector::unmuteMetrics(int contextGroupId)
	{
	}
	
	void V8Inspector::beginUserGesture()
	{
	}
	
	void V8Inspector::endUserGesture()
	{
	}
	
	std::unique_ptr<v8_inspector::StringBuffer> V8Inspector::valueSubtype(v8::Local<v8::Value>)
	{
		return nullptr;
	}
	
	bool V8Inspector::formatAccessorsAsProperties(v8::Local<v8::Value>)
	{
		return false;
	}
	
	bool V8Inspector::isInspectableHeapObject(v8::Local<v8::Object>)
	{
		return true;
	}
	
	v8::Local<v8::Context> V8Inspector::ensureDefaultContextInGroup(int contextGroupId)
	{
		return context;
	}
	
	void V8Inspector::beginEnsureAllContextsInGroup(int contextGroupId)
	{
	}
	
	void V8Inspector::endEnsureAllContextsInGroup(int contextGroupId)
	{
	}
	
	void V8Inspector::installAdditionalCommandLineAPI(v8::Local<v8::Context>,
															v8::Local<v8::Object>)
	{
	}
	
	void V8Inspector::consoleAPIMessage(int contextGroupId,
											  v8::Isolate::MessageErrorLevel level,
											  const v8_inspector::StringView& message,
											  const v8_inspector::StringView& url, unsigned lineNumber,
											  unsigned columnNumber, v8_inspector::V8StackTrace*)
	{
	}
	
	v8::MaybeLocal<v8::Value> V8Inspector::memoryInfo(v8::Isolate*, v8::Local<v8::Context>)
	{
		return v8::MaybeLocal<v8::Value>();
	}
	
	void V8Inspector::consoleTime(const v8_inspector::StringView& title)
	{
	}
	
	void V8Inspector::consoleTimeEnd(const v8_inspector::StringView& title)
	{
	}
	
	void V8Inspector::consoleTimeStamp(const v8_inspector::StringView& title)
	{
	}
	
	void V8Inspector::consoleClear(int contextGroupId)
	{
	}
	
	double V8Inspector::currentTimeMS()
	{
		return 0;
	}
	
	void V8Inspector::startRepeatingTimer(double, V8Inspector::TimerCallback, void* data)
	{
	}
	
	void V8Inspector::cancelTimer(void* data)
	{
	}
	
	bool V8Inspector::canExecuteScripts(int contextGroupId)
	{
		return true;
	}
	
	void V8Inspector::maxAsyncCallStackDepthChanged(int depth)
	{
	}
	
	std::unique_ptr<v8_inspector::StringBuffer> V8Inspector::resourceNameToUrl(const v8_inspector::StringView& resourceName)
	{
		return nullptr;
	}

	V8InspectorChannel::V8InspectorChannel(CPP::Net::WebSocketConnection &frontendConnection)
	: client(frontendConnection)
	{
	}

	V8InspectorChannel::~V8InspectorChannel()
	{
	}

	void V8InspectorChannel::sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message)
	{
		sendMessage(message->string ());
	}

	void V8InspectorChannel::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message)
	{
		sendMessage(message->string());
	}

	void V8InspectorChannel::flushProtocolNotifications()
	{
	}

	void V8InspectorChannel::sendMessage(const v8_inspector::StringView &message)
	{
		if (message.is8Bit() == true)
		{
			String msg((char *)message.characters8(), message.length());
			
			//std::cout << "- Send Backend message:\n"<< msg << std::endl;
			
			client.send(msg);
		}
		else
		{
			const uint16_t *c16msg = message.characters16();
			std::unique_ptr<char[]> cmsg(RJNEW char[message.length() + 1]);

			for (RJINT iIdx = 0; iIdx < message.length (); iIdx++)
				cmsg.get ()[iIdx] = c16msg[iIdx];

			cmsg.get()[message.length()] = '\0';

			String msg(cmsg.get ());

			//std::cout << "- Send Backend message:\n"<< msg << std::endl;

			client.send(msg);
		}
	}
}
#endif

