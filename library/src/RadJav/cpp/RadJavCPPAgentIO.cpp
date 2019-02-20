/*
	MIT-LICENSE
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

#include "cpp/RadJavCPPAgent.h"
#include "cpp/RadJavCPPAgentIO.h"

#ifdef USE_INSPECTOR
#include "v8-inspector.h"
#include "v8-platform.h"

#include <unicode/unistr.h>

#include "cpp/RadJavCPPNetWebServerUpgradable.h"
#include "v8/RadJavV8JavascriptEngine.h"

#ifdef __POSIX__
	#include <unistd.h>  // setuid, getuid
#endif  // __POSIX__

namespace RadJAV
{
	namespace V8inspector
	{

		namespace {
			using namespace v8;
			using namespace icu;
			using v8_inspector::StringBuffer;
			using v8_inspector::StringView;
			template<typename Transport>
			using TransportAndIo = std::pair<Transport*, InspectorIo*>;

			//TODO: implement
			std::string GetProcessTitle() {
				return "v8inspector";
			}
			//std::string ScriptPath(const std::string& script_name);

			std::string StringViewToUtf8(const StringView& view) {
				if (view.is8Bit()) {
					return std::string(reinterpret_cast<const char*>(view.characters8()),
						view.length());
				}
				const uint16_t* source = view.characters16();
				const UChar* unicodeSource = reinterpret_cast<const UChar*>(source);
				size_t result_length = view.length() * sizeof(*source);
				std::string result(result_length, '\0');
				UnicodeString utf16(unicodeSource, view.length());
				bool done = false;
				while (!done) {
					CheckedArrayByteSink sink(&result[0], result_length);
					utf16.toUTF8(sink);
					result_length = sink.NumberOfBytesAppended();
					result.resize(result_length);
					done = !sink.Overflowed();
				}
				return result;
			}

			//TODO: check
			//HandleSyncCloseCb(uv_handle_t* handle) {
			//CloseAsyncAndLoop(uv_async_t* async) {
			//ReleasePairOnAsyncClose(uv_handle_t* async) {
		} //anonymous namespace

		class IoSessionDelegate : public InspectorSessionDelegate {
		public:
			explicit IoSessionDelegate(InspectorIo* io) : io_(io) { }
			bool WaitForFrontendMessageWhilePaused() override;
			void SendMessageToFrontend(const v8_inspector::StringView& message) override;
		private:
			InspectorIo * io_;
		};

		void InterruptCallback(Isolate*, void* agent) {
			InspectorIo* io = static_cast<Agent*>(agent)->io();
			if (io != nullptr)
				io->DispatchMessages();
		}

		class DispatchMessagesTask : public Task {
		public:
			explicit DispatchMessagesTask(Agent* agent) : agent_(agent) {}

			void Run() override {
				InspectorIo* io = agent_->io();
				if (io != nullptr)
					io->DispatchMessages();
			}

		private:
			Agent* agent_;
		};


		InspectorIo::InspectorIo(Isolate* isolate, Platform* platform, const std::string& path,
			std::string host_name, bool wait_for_connect, std::string file_path, Agent *agent)
			: state_(State::kNew)
			, isolate_(isolate)
			, platform_(platform)
			, dispatching_messages_(false)
			, session_id_(0)
			, script_name_(path)
			, wait_for_connect_(wait_for_connect)
			, host_name_(host_name)
			, port_(0)
			, file_path_(file_path)
			, agent_(agent)
		{
			delegate_ = RJNEW InspectorIoDelegate(this, file_path_, script_name_, wait_for_connect_);
			//server_ = RJNEW RadJAV::CPP::Net::WebServerUpgradable(delegate_, file_path_, file_path_, wait_for_connect_);
		}

		InspectorIo::~InspectorIo()
		{
			if (server_)
			{
				server_->close();
				RJDELETE server_;
			}
			RJDELETE delegate_;
		}
		bool InspectorIo::Start() {
			assert(this->delegate_);
			//server_->setIOCallback(this->delegate_);
			PostIncomingMessage(InspectorAction::kStartSession, 0, "");
			//server_->listen(isolate_, &thread_start_sem_/*, platform_*/);

			thread_start_sem_.Wait();

			if (state_ == State::kError) {
				return false;
			}

			state_ = State::kAccepting;

			if (wait_for_connect_) {
				DispatchMessages();
			}
			return true;
		}

		void InspectorIo::Stop() {
			assert(state_ == State::kAccepting || state_ == State::kConnected);
			Write(TransportAction::kKill, 0, StringView());
			server_->close();
			RJDELETE server_;
			server_ = nullptr;
			state_ = State::kShutDown;
			DispatchMessages();
		}

		bool InspectorIo::IsConnected() {
			return delegate_ != nullptr && delegate_->IsConnected();
		}

		bool InspectorIo::IsStarted() {
			return platform_ != nullptr;
		}

		void InspectorIo::WaitForDisconnect() {
			if (state_ == State::kAccepting)
				state_ = State::kDone;
			if (state_ == State::kConnected) {
				state_ = State::kShutDown;
				Write(TransportAction::kStop, 0, StringView());
				fprintf(stderr, "Waiting for the debugger to disconnect...\n");
				fflush(stderr);
				agent_->RunMessageLoop();
			}
		}

		void InspectorIo::ThreadMain(void* io) {
			static_cast<InspectorIo*>(io)->ThreadMain<RadJAV::CPP::Net::WebServerUpgradable>();
		}

		template <typename Transport>
		void InspectorIo::IoThreadAsyncCb(Transport* transport, InspectorIo* io) {

			MessageQueue<TransportAction> outgoing_message_queue;
			io->SwapBehindLock(&io->outgoing_message_queue_, &outgoing_message_queue);
			for (const auto& outgoing : outgoing_message_queue) {
				switch (std::get<0>(outgoing)) {
				case TransportAction::kKill:
					transport->TerminateConnections();
					// Fallthrough
				case TransportAction::kStop:
					transport->Stop(nullptr);
					break;
				case TransportAction::kSendMessage:
					std::string message = StringViewToUtf8(std::get<2>(outgoing)->string());
					//fprintf(stderr, "%d %s sending message %s \n", __LINE__, __FILE__, message.c_str());
					transport->Send(std::get<1>(outgoing), message);
					break;
				}
			}
		}

		template<typename Transport>
		void InspectorIo::ThreadMain() {
			//err = uv_async_init(&loop, &thread_req_, IoThreadAsyncCb<Transport>);
			//TODO: check if script_path needed at all
			//std::string script_path = ScriptPath(&loop, script_name_);
			InspectorIoDelegate delegate(this, /*script_path*/script_name_, script_name_, wait_for_connect_);
			delegate_ = &delegate;

			/*Transport server(&delegate, host_name_, port_, fopen(file_path_.c_str(), "w"));
			server.listen(this->isolate_, &thread_start_sem_);
			if (!server.isAlive) {
				state_ = State::kError;  // Safe, main thread is waiting on semaphore
				thread_start_sem_.Notify();
				return;
			}
			port_ = server.port;  // Safe, main thread is waiting on semaphore.
			if (!wait_for_connect_) {
				thread_start_sem_.Notify();
			}*/

			delegate_ = nullptr;
		}

		template <typename ActionType>
		bool InspectorIo::AppendMessage(MessageQueue<ActionType>* queue,
			ActionType action, int session_id,
			std::unique_ptr<StringBuffer> buffer) {
			state_lock_.lock();
			//uv_mutex_lock(&state_lock_);
			bool trigger_pumping = queue->empty();
			queue->push_back(std::make_tuple(action, session_id, std::move(buffer)));
			state_lock_.unlock();
			//uv_mutex_unlock(&state_lock_);
			return trigger_pumping;
		}

		template <typename ActionType>
		void InspectorIo::SwapBehindLock(MessageQueue<ActionType>* vector1,
			MessageQueue<ActionType>* vector2) {
			state_lock_.lock();
			//uv_mutex_lock(&state_lock_);
			vector1->swap(*vector2);
			state_lock_.unlock();
			//uv_mutex_unlock(&state_lock_);
		}

		void InspectorIo::PostIncomingMessage(InspectorAction action, int session_id, const std::string& message) {
			if (AppendMessage(&incoming_message_queue_, action, session_id, V8inspector::Utf8ToStringView(message))) {
				platform_->CallOnForegroundThread(isolate_, new DispatchMessagesTask(agent_));
				isolate_->RequestInterrupt(InterruptCallback, agent_);
			}

			NotifyMessageReceived();
		}

		std::vector<std::string> InspectorIo::GetTargetIds() const {
			return delegate_ ? delegate_->GetTargetIds() : std::vector<std::string>();
		}

		void InspectorIo::WaitForFrontendMessageWhilePaused() {
			dispatching_messages_ = false;
			std::unique_lock<std::mutex> lck(state_lock_);
			if (incoming_message_queue_.empty())
				incoming_message_cond_.wait(lck);
			//uv_cond_wait(&incoming_message_cond_, &state_lock_);
		}

		void InspectorIo::NotifyMessageReceived() {
			std::unique_lock<std::mutex> lck(state_lock_);
			incoming_message_cond_.notify_all();
			//uv_cond_broadcast(&incoming_message_cond_);
		}

		void InspectorIo::DispatchMessages() {
			// This function can be reentered if there was an incoming message while
			// V8 was processing another inspector request (e.g. if the user is
			// evaluating a long-running JS code snippet). This can happen only at
			// specific points (e.g. the lines that call inspector_ methods)
			if (dispatching_messages_)
				return;
			dispatching_messages_ = true;
			bool had_messages = false;
			do {
				if (dispatching_message_queue_.empty())
					SwapBehindLock(&incoming_message_queue_, &dispatching_message_queue_);
				had_messages = !dispatching_message_queue_.empty();
				while (!dispatching_message_queue_.empty()) {
					MessageQueue<InspectorAction>::value_type task;
					std::swap(dispatching_message_queue_.front(), task);
					dispatching_message_queue_.pop_front();
					StringView message = std::get<2>(task)->string();
					switch (std::get<0>(task)) {
					case InspectorAction::kStartSession:
						assert(session_delegate_ == nullptr);
						session_id_ = std::get<1>(task);
						state_ = State::kConnected;
						fprintf(stderr, "Debugger attached.\n");
						session_delegate_ = std::unique_ptr<InspectorSessionDelegate>(
							RJNEW IoSessionDelegate(this));
						agent_->Connect(session_delegate_.get());
						break;
					case InspectorAction::kEndSession:
						assert(session_delegate_ != nullptr);
						if (state_ == State::kShutDown) {
							state_ = State::kDone;
						}
						else {
							state_ = State::kAccepting;
						}
						agent_->Disconnect();
						session_delegate_.reset();
						break;
					case InspectorAction::kSendMessage:
						agent_->Dispatch(message);
						break;
					}
				}
			} while (had_messages);
			dispatching_messages_ = false;
		}

		// static
		void InspectorIo::MainThreadReqAsyncCb(InspectorIo* io) {
			if (io != nullptr)
				io->DispatchMessages();
		}

		//TODO: implement async send instead of synchronous write here
		void InspectorIo::Write(TransportAction action, int session_id,
			const StringView& inspector_message) {
			if (state_ == State::kShutDown)
				return;
			//AppendMessage(&outgoing_message_queue_, action, session_id,
			//	StringBuffer::create(inspector_message));

			//TODO: send from outgoing_message_queue_
			//int err = uv_async_send(&thread_req_);
			this->delegate_->sendMessageToFrontend(StringViewToUtf8(inspector_message));
		}

		InspectorIoDelegate::InspectorIoDelegate(InspectorIo* io,
			const std::string& script_path,
			const std::string& script_name,
			bool wait)
			: io_(io),
			connected_(false),
			session_id_(0),
			script_name_(script_name),
			script_path_(script_path),
			waiting_(wait),
			string_session_id_("")
		{
		}


		bool InspectorIoDelegate::StartSession(int session_id,
			const std::string& target_id) {
			if (connected_)
				return false;
			connected_ = true;
			io_->PostIncomingMessage(InspectorAction::kStartSession, session_id, "");
			return true;
		}

		void InspectorIoDelegate::MessageReceived(int session_id,
			const std::string& message) {
			if (waiting_) {
				if (message.find("\"Runtime.runIfWaitingForDebugger\"") !=
					std::string::npos) {
					waiting_ = false;
					io_->ResumeStartup();
				}
			}
			io_->PostIncomingMessage(InspectorAction::kSendMessage, session_id,
				message);
		}

		void InspectorIoDelegate::EndSession(int session_id) {
			connected_ = false;
			io_->PostIncomingMessage(InspectorAction::kEndSession, session_id, "");
		}

		std::vector<std::string> InspectorIoDelegate::GetTargetIds() {
			return { target_id_ };
		}

		std::string InspectorIoDelegate::GetTargetTitle(const std::string& id) {
			return script_name_.empty() ? GetProcessTitle() : script_name_;
		}

		std::string InspectorIoDelegate::GetTargetUrl(const std::string& id) {
			return "file://" + script_path_;
		}

		void InspectorIoDelegate::ServerDone() {
			io_->ServerDone();
		}

		bool IoSessionDelegate::WaitForFrontendMessageWhilePaused() {
			io_->WaitForFrontendMessageWhilePaused();
			return true;
		}

		void InspectorIoDelegate::sendMessageToInspector(RadJAV::String message)
		{
			//TODO: send message
			if (io_)
			{
				//v8_inspector::StringView v((const uint8_t*)message.c_str(), message.length());
				printf("toInspector: %s\n", message.c_str());
				io_->PostIncomingMessage(InspectorAction::kSendMessage, session_id_, message);
			}
		}

		void InspectorIoDelegate::sendMessageToFrontend(RadJAV::String message)
		{
			//TODO: send message
			if (this->io_->server_)
			{
				v8_inspector::StringView v((const uint8_t*)message.c_str(), message.length());
				printf("       from: %s\n", message.c_str());
				this->io_->server_->send(this->getSessionID(), message);
			}
		}

		void IoSessionDelegate::SendMessageToFrontend(
			const v8_inspector::StringView& message) {
			io_->Write(TransportAction::kSendMessage, io_->session_id_, message);
		}

	} // namespace inspector
}
#endif

