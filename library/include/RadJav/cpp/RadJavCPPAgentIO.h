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

#include <string>
#include "v8-inspector.h"

#include <v8.h>

#include <deque>
#include <memory>
#include <condition_variable>
#include <mutex>
#include "RadJavCPPNetWebServerUpgradable.h"
#include "RadJavCPPAgent.h"
#include "RadJavCPPDBKeyValueStorage.h"
#include "v8/RadJavV8JavascriptEngine.h"

namespace RadJAV {
	namespace CPP {
		namespace Net {
			class WebServerUpgradable;
		}
	}
}

namespace inspector
{
	class InspectorIo;
	class Agent;
	using namespace v8;

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

	// Passed to InspectorSocketServer to handle WS inspector protocol events,
	// mostly session start, message received, and session end.
	class InspectorIoDelegate : public SocketServerDelegate {
	public:
		InspectorIoDelegate(InspectorIo* io, const std::string& script_path,
			const std::string& script_name, bool wait);
		// Calls PostIncomingMessage() with appropriate InspectorAction:
		//   kStartSession
		bool StartSession(int session_id, const std::string& target_id) override;
		//   kSendMessage
		void MessageReceived(int session_id, const std::string& message) override;
		//   kEndSession
		void EndSession(int session_id) override;

		std::vector<std::string> GetTargetIds() override;
		std::string GetTargetTitle(const std::string& id) override;
		std::string GetTargetUrl(const std::string& id) override;
		bool IsConnected() { return connected_; }
		void ServerDone() override;

		virtual void sendMessageToInspector(RadJAV::String message);
		virtual void sendMessageToFrontend(RadJAV::String message);
		static void SendMessageToInspector(InspectorIoDelegate* self, const RadJAV::String& message)
		{
			if (self)
			{
				return self->sendMessageToInspector(message);
			}
		}

		void setSessionID(std::string sessionId)
		{
			string_session_id_ = sessionId;
		}

		RadJAV::String getSessionID()
		{
			return string_session_id_;
		}
	private:
		InspectorIo * io_;
		bool connected_;
		std::string string_session_id_;
		int session_id_;
		const std::string script_name_;
		const std::string script_path_;
		const std::string target_id_;
		bool waiting_;
	};

	class InspectorSessionDelegate;

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


	enum class InspectorAction
	{
		kStartSession,
		kEndSession,
		kSendMessage
	};

	// kKill closes connections and stops the server, kStop only stops the server
	enum class TransportAction
	{
		kKill,
		kSendMessage,
		kStop
	};

	class InspectorIo
	{
	public:
		InspectorIo(Isolate* isolate, Platform* platform,
		            const std::string& path, std::string host_name, bool wait_for_connect, std::string file_path_,
		            Agent* agent);

		~InspectorIo();
		// Start the inspector agent thread, waiting for it to initialize,
		// and waiting as well for a connection if wait_for_connect.
		bool Start();
		// Stop the inspector agent thread.
		void Stop();

		bool IsStarted();
		bool IsConnected();

		void WaitForDisconnect();
		// Called from thread to queue an incoming message and trigger
		// DispatchMessages() on the main thread.
		void PostIncomingMessage(InspectorAction action, int session_id,
		                         const std::string& message);

		void ResumeStartup()
		{
			thread_start_sem_.Notify();
		}

		void ServerDone()
		{
			//TODO: check if action required
			//uv_close(reinterpret_cast<uv_handle_t*>(&thread_req_), nullptr);
		}

		int port() const { return port_; }
		std::string host() const { return host_name_; }
		std::vector<std::string> GetTargetIds() const;
		RadJAV::CPP::Net::WebServerUpgradable* server_;

	private:
		template <typename Action>
		using MessageQueue = std::deque<std::tuple<Action, int,
			std::unique_ptr<v8_inspector::StringBuffer>>>;

		enum class State
		{
			kNew,
			kAccepting,
			kConnected,
			kDone,
			kError,
			kShutDown
		};

		// Callback for main_thread_req_'s uv_async_t
		static void MainThreadReqAsyncCb(InspectorIo* io);

		// Wrapper for agent->ThreadMain()
		static void ThreadMain(void* io);

		// Runs a uv_loop_t
		template <typename Transport>
		void ThreadMain();
		// Called by ThreadMain's loop when triggered by thread_req_, writes
		// messages from outgoing_message_queue to the InspectorSocketServer
		template <typename Transport>
		static void IoThreadAsyncCb(Transport* transport, InspectorIo* io);

		void DispatchMessages();
		// Write action to outgoing_message_queue, and wake the thread
		void Write(TransportAction action, int session_id,
		           const v8_inspector::StringView& message);
		// Thread-safe append of message to a queue. Return true if the queue
		// used to be empty.
		template <typename ActionType>
		bool AppendMessage(MessageQueue<ActionType>* vector, ActionType action,
		                   int session_id,
		                   std::unique_ptr<v8_inspector::StringBuffer> buffer);
		// Used as equivalent of a thread-safe "pop" of an entire queue's content.
		template <typename ActionType>
		void SwapBehindLock(MessageQueue<ActionType>* vector1,
		                    MessageQueue<ActionType>* vector2);
		// Wait on incoming_message_cond_
		void WaitForFrontendMessageWhilePaused();
		// Broadcast incoming_message_cond_
		void NotifyMessageReceived();

		// Used by Start() to wait for thread to initialize, or for it to initialize
		// and receive a connection if wait_for_connect was requested.
		Semaphore thread_start_sem_;

		InspectorIoDelegate* delegate_;
		State state_;

		std::unique_ptr<InspectorSessionDelegate> session_delegate_{};
		Platform* platform_;
		Isolate* isolate_;

		// Message queues
		std::condition_variable incoming_message_cond_;
		std::mutex state_lock_;

		MessageQueue<InspectorAction> incoming_message_queue_;
		MessageQueue<InspectorAction> dispatching_message_queue_;
		//TODO: check async send usage
		//MessageQueue<TransportAction> outgoing_message_queue_;

		bool dispatching_messages_;
		int session_id_;

		std::string script_name_;
		std::string script_path_;
		std::string host_name_;
		std::string file_path_;
		Agent* agent_;
		const bool wait_for_connect_;
		int port_;
		
		friend class DispatchMessagesTask;
		friend class IoSessionDelegate;
		friend void InterruptCallback(Isolate*, void* agent);
	};
} // namespace inspector

#endif  // _RADJAV_CPP_AGENT_IO_H_
