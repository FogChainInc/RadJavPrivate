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

	#include <queue>
	#include <mutex>


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

		template<class T>
		class ThreadSafeQueue
		{
		public:
			void push( const T& value )
			{
				std::lock_guard<std::mutex> guard(accessMutex);
				data.push(value);
			}

			void pop()
			{
				std::lock_guard<std::mutex> guard(accessMutex);
				data.pop();
			}

			const T& front()
			{
				std::lock_guard<std::mutex> guard(accessMutex);
				return data.front();
			}
			
			bool empty()
			{
				std::lock_guard<std::mutex> guard(accessMutex);
				return data.empty();
			}
			
			size_t size()
			{
				std::lock_guard<std::mutex> guard(accessMutex);
				return data.size();
			}
			
		private:
			std::mutex accessMutex;
			std::queue<T> data;
		};
		
		using FrontEndMessageQueue = ThreadSafeQueue<String>;
		
		class V8InspectorChannel;

		/// The V8 inspector.
		class RADJAV_EXPORT V8Inspector: public v8_inspector::V8InspectorClient
		{
			public:
				V8Inspector(v8::Isolate *isolate, v8::Local<v8::Context> context);
				~V8Inspector();

				/**
				 * @brief Start WebSocketServer and listen for a connection.
				 */
				void start(String ip, RJINT port);
			
				/**
				 * @brief Cleanup.
				 */
				void close();
			
				/**
				 * @brief Wait until CDT(Frontend) will connect.
				 * @details It will block until all control messages processed between Backend and Frontend(CDT).
				 */
				void waitForConnection();
				void pauseOnStart();
			
				/**
				 * @brief Process CDT(Frontend) requests on main thread
				 * @details Process all outstanding CDT(Frontend) requests on main thread. We need to call this in each main loop step,
				 *  and after each script compilation.
				 */
				void dispatchFrontendMessages();
			
			protected:
				// From V8InspectorClient
				void runMessageLoopOnPause(int contextGroupId);
				void quitMessageLoopOnPause();
				void runIfWaitingForDebugger(int contextGroupId);
				void muteMetrics(int contextGroupId);
				void unmuteMetrics(int contextGroupId);
				void beginUserGesture();
				void endUserGesture();
				std::unique_ptr<v8_inspector::StringBuffer> valueSubtype(v8::Local<v8::Value>);
				bool formatAccessorsAsProperties(v8::Local<v8::Value>);
				bool isInspectableHeapObject(v8::Local<v8::Object>);
				v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId);
				void beginEnsureAllContextsInGroup(int contextGroupId);
				void endEnsureAllContextsInGroup(int contextGroupId);
				void installAdditionalCommandLineAPI(v8::Local<v8::Context>, v8::Local<v8::Object>);
				void consoleAPIMessage(int contextGroupId,
								   v8::Isolate::MessageErrorLevel level,
								   const v8_inspector::StringView& message,
								   const v8_inspector::StringView& url, unsigned lineNumber,
								   unsigned columnNumber, v8_inspector::V8StackTrace*);
				v8::MaybeLocal<v8::Value> memoryInfo(v8::Isolate*, v8::Local<v8::Context>);
				void consoleTime(const v8_inspector::StringView& title);
				void consoleTimeEnd(const v8_inspector::StringView& title);
				void consoleTimeStamp(const v8_inspector::StringView& title);
				void consoleClear(int contextGroupId);
				double currentTimeMS();
			
				typedef void (*TimerCallback)(void*);
			
				void startRepeatingTimer(double, TimerCallback, void* data);
				void cancelTimer(void* data);
				bool canExecuteScripts(int contextGroupId);
				void maxAsyncCallStackDepthChanged(int depth);
				std::unique_ptr<v8_inspector::StringBuffer> resourceNameToUrl(const v8_inspector::StringView& resourceName);

			protected:
				RJBOOL isPaused;
				CPP::Net::WebServerUpgradable *server;
				v8::Isolate *isolate;
				v8::Local<v8::Context> context;
				std::unique_ptr<v8_inspector::V8Inspector> inspector;
				std::unique_ptr<v8_inspector::V8InspectorSession> session;
				V8InspectorChannel *channel;
				FrontEndMessageQueue frontendPendingMessages;
				bool ready;
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
