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
#include "cpp/RadJavCPPNetWebServerUpgradable.h"

#ifdef USE_V8
namespace RadJAV
{
	V8Inspector::V8Inspector(v8::Isolate *isolate, v8::Local<v8::Context> context)
	{
		isPaused = false;
		server = NULL;
		this->isolate = isolate;
		this->context = context;
		inspector = NULL;
		channel = NULL;
	}

	V8Inspector::~V8Inspector()
	{
		close();

		DELETEOBJ(server); /// @fixme This is causing crashes upon deletion. Not sure why yet.
	}

	void V8Inspector::start(String ip, RJINT port)
	{
		session.reset();
		inspector = v8_inspector::V8Inspector::create (isolate, this);
		// Seems that we didn't refer to it
		//context->SetAlignedPointerInEmbedderData(kInspectorClientIndex, this);
		inspector->contextCreated(v8_inspector::V8ContextInfo(context, 1, v8_inspector::StringView()));

		server = RJNEW CPP::Net::WebServerUpgradable(ip, port, true);

		server->cppOn("webSocketReceive", [&](Array<void *> args) -> void *
			{
				// We just put pending frontend(CDT) message into thread safe queue for future processing
				frontendPendingMessages.push(String((*(String*)args.at(1))));
				
				// Old implementation, this leads to crash due to access to Isolate from asio thread
				/*
				String *id = (String *)args.at (0);
				String *message = (String *)args.at (1);

				std::unique_ptr<uint16_t[]> msgBuffer = createMessageBuffer_uint16 (*message);
				v8_inspector::StringView msg(msgBuffer.get(), message->size());
				session->dispatchProtocolMessage(msg);
				 */
				
				return (NULL);
			});
		server->cppOn("httpReceive", [&](Array<void *> args) -> void *
		{
			String *id = (String *)args.at(0);
			String *message = (String *)args.at(1);

			String result = "";
			
			result += "<!DOCTYPE html>";
			result += "<html>";
			result += "<body>";
			result += "You requested ws://127.0.0.1:9229/";
			result += "</body>";
			result += "</html>";

			String *output = RJNEW String(result);

			return (output);
		});
		server->cppOn("httpSend", [&](Array<void *> args) -> void *
		{
			String *id = (String *)args.at(0);
			String *message = (String *)args.at(1);

			return (NULL);
		});
		server->cppOn("webSocketUpgrade", [&](Array<void *> args) -> void *
		{
			if (server->numWebSocketConnections () > 0)
				return (RJNEW RJBOOL(false));

			String *id = (String *)args.at(0);

			channel = RJNEW V8InspectorChannel(server, *id);
			session = inspector->connect(1, channel, v8_inspector::StringView());

			String pauseReason = "Break on start";
			std::unique_ptr<uint16_t[]> msgBuffer = createMessageBuffer_uint16(pauseReason);
			v8_inspector::StringView msg(msgBuffer.get(), pauseReason.size());
			session->schedulePauseOnNextStatement(msg, msg);

			return (RJNEW RJBOOL(true));
		});
		server->cppOn("webSocketClose", [&](Array<void *> args) -> void *
		{
			String *id = (String *)args.at(0);

			isPaused = false;
			session.reset();

			return (NULL);
		});

		server->listen();
	}

	void V8Inspector::close()
	{
		session.reset();
		DELETEOBJ(channel);
		server->close();
	}

	void V8Inspector::waitForConnection()
	{
		while (server->numWebSocketConnections() < 1)
			CPP::OS::sleep(100);
	}

	void V8Inspector::pauseOnStart()
	{
		String message = "Break on start";
		std::unique_ptr<uint16_t[]> msgBuffer = createMessageBuffer_uint16(message);
		v8_inspector::StringView msg(msgBuffer.get(), message.size());
		
		session->schedulePauseOnNextStatement(msg, msg);
	}

	void V8Inspector::dispatchFrontendMessages()
	{
		while (frontendPendingMessages.size())
		{
			const String& message = frontendPendingMessages.front();
			
			std::unique_ptr<uint16_t[]> msgBuffer = createMessageBuffer_uint16 (message);
			v8_inspector::StringView msg(msgBuffer.get(), message.size());

			session->dispatchProtocolMessage(msg);
			
			frontendPendingMessages.pop();
		}
	}

	std::unique_ptr<uint16_t[]> V8Inspector::createMessageBuffer_uint16(String message)
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

	void V8Inspector::runMessageLoopOnPause(int contextGroupId)
	{
		isPaused = true;

		while (isPaused == true)
		{
			dispatchFrontendMessages();
			CPP::OS::sleep(100);
		}
	}
	
	void V8Inspector::quitMessageLoopOnPause()
	{
		isPaused = false;
	}
	
	void V8Inspector::runIfWaitingForDebugger(int contextGroupId)
	{
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

	V8InspectorChannel::V8InspectorChannel(CPP::Net::WebServerUpgradable *server, String clientId)
	{
		this->server = server;
		this->clientId = clientId;
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
			server->send(clientId, msg);
		}
		else
		{
			const uint16_t *c16msg = message.characters16();
			std::unique_ptr<char[]> cmsg(RJNEW char[message.length() + 1]);

			for (RJINT iIdx = 0; iIdx < message.length (); iIdx++)
				cmsg.get ()[iIdx] = c16msg[iIdx];

			cmsg.get()[message.length()] = '\0';

			String msg(cmsg.get ());

			server->send(clientId, msg);
		}
	}
}
#endif

