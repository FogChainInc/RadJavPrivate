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
#ifndef _RADJAV_V8_INSPECTOR_H_
	#define _RADJAV_V8_INSPECTOR_H_

	#include "RadJavPreprocessor.h"

	#include <v8.h>
	#include <v8-inspector.h>

	#include "RadJavString.h"
	#include "RadJavArray.h"
	#include "RadJavHashMap.h"

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				class WebServerUpgradable;
			}
		}

		enum
		{
			kModuleEmbedderDataIndex,
			kInspectorClientIndex
		};

		class V8InspectorChannel;

		/// The V8 inspector.
		class RADJAV_EXPORT V8Inspector: public v8_inspector::V8InspectorClient
		{
			public:
				V8Inspector(v8::Isolate *isolate, v8::Local<v8::Context> context);
				~V8Inspector();

				void start(String ip, RJINT port);
				void close();

				virtual void runMessageLoopOnPause(int contextGroupId) override;
				virtual void quitMessageLoopOnPause() override;
				v8::Local<v8::Context> ensureDefaultContextInGroup(int groupId) override;

				void waitForConnection();

				std::unique_ptr<uint16_t[]> createMessageBuffer_uint16(String message);

			protected:
				RJBOOL isPaused;
				CPP::Net::WebServerUpgradable *server;
				v8::Isolate *isolate;
				v8::Local<v8::Context> context;
				std::unique_ptr<v8_inspector::V8Inspector> inspector;
				std::unique_ptr<v8_inspector::V8InspectorSession> session;
				V8InspectorChannel *channel;
		};

		/// The network connection between the WebSocket server and the CDT.
		class RADJAV_EXPORT V8InspectorChannel: public v8_inspector::V8Inspector::Channel
		{
			public:
				V8InspectorChannel(CPP::Net::WebServerUpgradable *server, String clientId);
				~V8InspectorChannel();

				void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) override;
				void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;

				void flushProtocolNotifications() override;

				void sendMessage(const v8_inspector::StringView &message);

			protected:
				CPP::Net::WebServerUpgradable *server;
				String clientId;
		};
	}
#endif
