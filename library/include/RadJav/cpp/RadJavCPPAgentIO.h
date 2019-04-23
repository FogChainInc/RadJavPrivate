/*
MIT-LICENSE
Copyright Node.js contributors. All rights reserved.
Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#ifndef _RADJAV_CPP_AGENT_IO_H_
#define _RADJAV_CPP_AGENT_IO_H_

#ifdef USE_INSPECTOR
#include <string>
#include "v8-inspector.h"

#include <v8.h>

#include <deque>
#include <memory>
#include <condition_variable>
#include <mutex>
//#include "RadJavCPPNetWebServerUpgradable.h"
#include "RadJavCPPAgent.h"
#include "v8/RadJavV8JavascriptEngine.h"

namespace RadJAV {
	namespace CPP {
		namespace Net {
			class WebServer;
		}
	}
}
namespace RadJAV
{
	namespace V8inspector
	{
		class InspectorIo;
		class Agent;
		using namespace v8;

		/**
		 * @class SocketServerDelegate
		 *
		 * @ingroup group_debug
		 *
		 * @brief Abstract Socket server delegate class used by the v8 Inspector Agent
		 *
		 * This class supposed to be inherited by actual Inspector IO delegate class to perform all
		 * communication actions between CDT client and V8 Inspector entities
		 *
		 */
		class SocketServerDelegate {
		public:
			virtual ~SocketServerDelegate() = default;
			virtual bool StartSession(int session_id, const std::string& target_id) = 0;
			virtual void EndSession(int session_id) = 0;
			virtual void MessageReceived(int session_id, const std::string& message) = 0;
			virtual std::vector<std::string> GetTargetIds() = 0;
			virtual std::string GetTargetTitle(const std::string& id) = 0;
			virtual std::string GetTargetUrl(const std::string& id) = 0;
			virtual void ServerDone() = 0;
		};

		/**
		 * @class InspectorIoDelegate
		 *
		 * @ingroup group_debug
		 *
		 * @brief socket server delegate implementation used by the v8 Inspector Agent
		 *
		 * Passed to InspectorSocketServer to handle WS inspector protocol events,
		 * mostly session start, message received, and session end.
		 */
		class InspectorIoDelegate : public SocketServerDelegate {
		public:

			/**
			 * @brief create inspector IO delegate object
			 *
			 * @param io pointer to Inspector IO class
			 *
			 * @param script_path path to executed script
			 *
			 * @param script_name name of the script to debug
			 *
			 * @param wait boolean flag that defines whether blocking wait for connection is required
			 */
			InspectorIoDelegate(InspectorIo* io, const std::string& script_path,
				const std::string& script_name, bool wait);

			/**
			 * @brief Call PostIncomingMessage() with appropriate InspectorAction: kStartSession
			 *
			 * @param session_id session ID
			 *
			 * @param target_id target ID
			 */
			bool StartSession(int session_id, const std::string& target_id) override;

			/**
			 * @brief Call PostIncomingMessage() with appropriate InspectorAction: kSendMessage
			 *
			 * @param session_id session ID
			 *
			 * @param message message to send
			 */
			void MessageReceived(int session_id, const std::string& message) override;

			/**
			 * @brief Call PostIncomingMessage() with appropriate InspectorAction: kEndSession
			 *
			 * @param session_id session ID to close
			 */
			void EndSession(int session_id) override;

			/**
			 * @brief return target IDs stored in the inner IO delegate
			 */
			std::vector<std::string> GetTargetIds() override;

			/**
			 * @brief return target title stored in the inner IO delegate
			 *
			 * @param id target ID
			 */
			std::string GetTargetTitle(const std::string& id) override;

			/**
			 * @brief return target URL by Id
			 *
			 * @param id target ID
			 */
			std::string GetTargetUrl(const std::string& id) override;

			/**
			 * @brief return if current delegate corrected
			 */
			bool IsConnected() { return connected_; }

			/**
			 * @brief call io->ServerDone()
			 */
			void ServerDone() override;

			/**
			 * @brief send message to inspector
			 *
			 * @param message message to send
			 */
			virtual void sendMessageToInspector(RadJAV::String message);

			/**
			 * @brief send message from inspector to frontend
			 *
			 * @param message message to send
			 */
			virtual void sendMessageToFrontend(RadJAV::String message);

			/**
			 * @brief send message to inspector
			 *
			 * @param self pointer to IO delegate object
			 *
			 * @param message message to send
			 */
			static void SendMessageToInspector(InspectorIoDelegate* self, const RadJAV::String& message)
			{
				if (self)
				{
					return self->sendMessageToInspector(message);
				}
			}

			/**
			 * @brief set session ID
			 *
			 * @param sessionId session ID
			 */
			void setSessionID(std::string sessionId)
			{
				string_session_id_ = sessionId;
			}

			/**
			 * @brief get session ID
			 *
			 */
			RadJAV::String getSessionID()
			{
				return string_session_id_;
			}
		private:
			/**
			 * @brief pointer to inspector IO delegate object
			 */
			InspectorIo * io_;
			/**
			 * Connected status
			 */
			bool connected_;

			/**
			 * @brief session ID in string format
			 */
			std::string string_session_id_;

			/**
			 * @brief session ID in integer format
			 */
			int session_id_;

			/**
			 * @brief script name
			 */
			const std::string script_name_;

			/**
			 * @brief script path
			 */
			const std::string script_path_;

			/**
			 * @brief target message id
			 */
			const std::string target_id_;

			/**
			 * @brief is waiting flag
			 */
			bool waiting_;
		};

		class InspectorSessionDelegate;

		/**
		 * @class Semaphore
		 *
		 * @ingroup group_debug
		 *
		 * @brief Simple semaphore implementation for V8 Inspector
		 *
		 * This class supposed to be used by V8 Inspector entities
		 *
		 */
		class Semaphore
		{
		public:
			void Notify() {
				std::unique_lock<decltype(mutex_)> lock(mutex_);
				++count_;
				condition_.notify_one();
			}

			void Wait() {
				std::unique_lock<decltype(mutex_)> lock(mutex_);
				while (!count_) // Handle spurious wake-ups.
					condition_.wait(lock);
				--count_;
			}

			bool Try_wait() {
				std::unique_lock<decltype(mutex_)> lock(mutex_);
				if (count_) {
					--count_;
					return true;
				}
				return false;
			}

		private:
			std::mutex mutex_;
			std::condition_variable condition_;
			unsigned long count_ = 0; // Initialized as locked.
		};

		/**
		 * @ingroup group_debug
		 *
		 * @brief enum with available inspector actions
		 *
		 */
		enum class InspectorAction
		{
			kStartSession, /// start new session action
			kEndSession, /// end existing session action
			kSendMessage /// send message
		};

		/**
		 * @ingroup group_debug
		 *
		 * @brief enum with available transport actions
		 *
		 */
		enum class TransportAction
		{
			kKill, /// action that closes connections and stops the server
			kSendMessage, ///action that sends service action
			kStop /// action that only stops the server
		};

		/**
		 * @class Semaphore
		 *
		 * @ingroup group_debug
		 *
		 * @brief Simple semaphore implementation for V8 Inspector
		 *
		 * This class supposed to be used by V8 Inspector entities
		 *
		 */
		class InspectorIo
		{
		public:
			/**
			 * @brief creaate inspector IO class
			 *
			 * @param isolate V8 isolate to debug
			 *
			 * @param platform V8 platform
			 *
			 * @param path string path to script
			 *
			 * @param host_name host name to listen for CDT connections
			 *
			 * @param wait_for_connect flag that indicates if inspector should block and wait for connection on start
			 *
			 * @param file_path_ script file path
			 *
			 * @param agent pointer to V8 Agent object
			 */

			InspectorIo(Isolate* isolate, Platform* platform,
				const std::string& path, std::string host_name, bool wait_for_connect, std::string file_path_,
				Agent* agent);

			~InspectorIo();

			/**
			 * @brief Start the inspector agent thread, waiting for it to initialize,
			 * and waiting as well for a connection if wait_for_connect.
			 */
			bool Start();

			/**
			 * @brief Stop the inspector agent thread.
			 */
			void Stop();

			/**
			 * @brief Check if listening started
			 */
			bool IsStarted();

			/**
			 * @brief Check if CDT connected
			 */
			bool IsConnected();

			/**
			 * @brief Wait for CDT disconnect
			 */
			void WaitForDisconnect();

			/**
			 * @brief process incoming message
			 *
			 * Called from thread to queue an incoming message and trigger DispatchMessages() on the main thread.
			 *
			 * @param action Inspector action
			 *
			 * @param session_id Session ID
			 *
			 * @param message string message to process
			 */
			void PostIncomingMessage(InspectorAction action, int session_id,
				const std::string& message);

			/**
			 * @brief Resume thread on startup by notifying the semaphore
			 */
			void ResumeStartup()
			{
				thread_start_sem_.Notify();
			}

			/**
			 * @brief finalize server at exit (currently empty)
			 */
			void ServerDone()
			{
				//TODO: check if action required
				//uv_close(reinterpret_cast<uv_handle_t*>(&thread_req_), nullptr);
			}

			/**
			 * @brief Get listening port
			 */
			int port() const { return port_; }

			/**
			 * @brief get listening host name
			 */
			std::string host() const { return host_name_; }

			/**
			 * @brief get target IDs
			 */
			std::vector<std::string> GetTargetIds() const;

			/**
			 * @brief underlying upgradable web server pointer
			 */
			RadJAV::CPP::Net::WebServer* server_;

		private:
			/**
			 * @brief message queue
			 */
			template <typename Action>
			using MessageQueue = std::deque<std::tuple<Action, int,
				std::unique_ptr<v8_inspector::StringBuffer>>>;

			/**
			 * @brief State of incoming connection
			 */
			enum class State
			{
				kNew,
				kAccepting,
				kConnected,
				kDone,
				kError,
				kShutDown
			};

			/**
			 * @brief Callback for main_thread_req_'s uv_async_t
			 */
			static void MainThreadReqAsyncCb(InspectorIo* io);

			/**
			 * @brief Wrapper for agent->ThreadMain()
			 */
			static void ThreadMain(void* io);

			/**
			 * @brief Main loop
			 */
			template <typename Transport>
			void ThreadMain();

			/**
			 * @brief IO thread message writer
			 *
			 * Called by ThreadMain's loop when triggered by thread_req_, writes
			 * messages from outgoing_message_queue to the InspectorSocketServer
			 */
			template <typename Transport>
			static void IoThreadAsyncCb(Transport* transport, InspectorIo* io);

			/**
			 * @brief Method to dispatch inbound and outbound messages
			 */
			void DispatchMessages();

			/**
			 * @brief Write action to outgoing_message_queue, and wake the thread
			 */
			void Write(TransportAction action, int session_id,
				const v8_inspector::StringView& message);

			/**
			 * @brief Thread-safe append of message to a queue. Return true if the queue used to be empty.
			 */
			template <typename ActionType>
			bool AppendMessage(MessageQueue<ActionType>* vector, ActionType action,
				int session_id,
				std::unique_ptr<v8_inspector::StringBuffer> buffer);

			/**
			 * @brief Used as equivalent of a thread-safe "pop" of an entire queue's content
			 */
			template <typename ActionType>
			void SwapBehindLock(MessageQueue<ActionType>* vector1,
				MessageQueue<ActionType>* vector2);

			/**
			 * @brief Wait on incoming_message_cond_
			 */
			void WaitForFrontendMessageWhilePaused();

			/**
			 * @brief Broadcast incoming_message_cond_
			 */
			void NotifyMessageReceived();

			/**
			 * @brief Simple synchronization semaphore
			 *
			 * Used by Start() to wait for thread to initialize, or for it to initialize
			 * and receive a connection if wait_for_connect was requested.
			 */
			Semaphore thread_start_sem_;

			/**
			 * @brief Inspector IO delegate
			 */
			InspectorIoDelegate* delegate_;

			/**
			 * @brief Current connection state
			 */
			State state_;

			/**
			 * @brief current session delegate
			 */
			std::unique_ptr<InspectorSessionDelegate> session_delegate_{};

			/**
			 * @brief V8 platform
			 */
			Platform* platform_;

			/**
			 * @brief V8 isolate
			 */
			Isolate* isolate_;

			/**
			 * @brief incoming messages queue condition variable
			 */
			std::condition_variable incoming_message_cond_;

			/**
			 * @brief messages queue mutex
			 */
			std::mutex state_lock_;

			/**
			 * @brief incoming messages queue
			 */
			MessageQueue<InspectorAction> incoming_message_queue_;

			/**
			 * @brief dispatching (service) messages queue
			 */
			MessageQueue<InspectorAction> dispatching_message_queue_;

			//TODO: check async send usage, outgoing_message_queue_ not used and returned for strict compilation
			/**
			 * @brief outgoing messages queue
			 */
			MessageQueue<TransportAction> outgoing_message_queue_;

			/**
			 * @brief Flag that indicates if dispatching messages available
			 */
			bool dispatching_messages_;

			/**
			 * @brief session ID
			 */
			int session_id_;

			/**
			 * @brief script name
			 */
			std::string script_name_;

			/**
			 * @brief script path
			 */
			std::string script_path_;

			/**
			 * @brief host name
			 */
			std::string host_name_;

			/**
			 * @brief file path
			 */
			std::string file_path_;

			/**
			 * @brief V8 Inspector Agent
			 */
			Agent* agent_;

			/**
			 * @brief wait for connect flag (used to block wait on start before script execution)
			 */
			const bool wait_for_connect_;

			/**
			 * @brief listening port
			 */
			int port_;

			friend class DispatchMessagesTask;
			friend class IoSessionDelegate;
			friend void InterruptCallback(Isolate*, void* agent);
		};
	} // namespace inspector
}

#endif
#endif  // _RADJAV_CPP_AGENT_IO_H_
