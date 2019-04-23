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

#include "libplatform/libplatform.h"

#include <unicode/platform.h>
#include <unicode/unistr.h>

#include "cpp/RadJavCPPNetWebServer.h"

#ifdef __POSIX__
#include <unistd.h>  // setuid, getuid
#endif  // __POSIX__

namespace RadJAV
{
	namespace V8inspector
	{

		std::unique_ptr<v8_inspector::StringBuffer> Utf8ToStringView(const std::string& message) {
			icu::UnicodeString utf16 =
				icu::UnicodeString::fromUTF8(icu::StringPiece(message.data(), message.length()));
			v8_inspector::StringView view(reinterpret_cast<const uint16_t*>(utf16.getBuffer()),
				utf16.length());
			return v8_inspector::StringBuffer::create(view);
		}

		std::unique_ptr<v8_inspector::StringBuffer> ToProtocolString(v8::Isolate *isolate, v8::Local<v8::Value> value) {
			if (value.IsEmpty() || value->IsNull() || value->IsUndefined() ||
				!value->IsString()) {
				return v8_inspector::StringBuffer::create(v8_inspector::StringView());
			}
			v8::Local<v8::String> string_value = v8::Local<v8::String>::Cast(value);
			size_t len = string_value->Length();
			std::basic_string<uint16_t> buffer(len, '\0');
			string_value->Write(isolate, &buffer[0], 0, len);
			return v8_inspector::StringBuffer::create(v8_inspector::StringView(buffer.data(), len));
		}

		namespace {
			using namespace v8;
			using namespace icu;

			const int CONTEXT_GROUP_ID = 1;

			class ChannelImpl final : public v8_inspector::V8Inspector::Channel {
			public:
				explicit ChannelImpl(v8_inspector::V8Inspector* inspector,
					InspectorSessionDelegate* delegate)
					: delegate_(delegate) {
					session_ = inspector->connect(1, this, v8_inspector::StringView());
				}

				virtual ~ChannelImpl() = default;

				void dispatchProtocolMessage(const v8_inspector::StringView& message) const
				{
					assert(session_);
					session_->dispatchProtocolMessage(message);
				}

				bool waitForFrontendMessage() const
				{
					return delegate_->WaitForFrontendMessageWhilePaused();
				}

				void schedulePauseOnNextStatement(const std::string& reason) const
				{
					std::unique_ptr<v8_inspector::StringBuffer> buffer = Utf8ToStringView(reason);
					session_->schedulePauseOnNextStatement(buffer->string(), buffer->string());
				}

				InspectorSessionDelegate* delegate() const
				{
					return delegate_;
				}

			private:
				void sendResponse(
					int callId,
					std::unique_ptr<v8_inspector::StringBuffer> message) override {
					sendMessageToFrontend(message->string());
				}

				void sendNotification(
					std::unique_ptr<v8_inspector::StringBuffer> message) override {
					sendMessageToFrontend(message->string());
				}

				void flushProtocolNotifications() override { }

				void sendMessageToFrontend(const v8_inspector::StringView& message) const
				{
					delegate_->SendMessageToFrontend(message);
				}

				InspectorSessionDelegate* const delegate_;
				std::unique_ptr<v8_inspector::V8InspectorSession> session_{};
			};

		}  // namespace

		class CBInspectorClient : public v8_inspector::V8InspectorClient {
		public:
			CBInspectorClient(Isolate* isolate, Platform* platform)
				: isolate_(isolate),
				platform_(platform),
				terminated_(false),
				running_nested_loop_(false) {
				client_ = v8_inspector::V8Inspector::create(isolate_, this);
			}

			void runMessageLoopOnPause(int context_group_id) override {
				(void)context_group_id;

				if (running_nested_loop_)
					return;
				terminated_ = false;
				running_nested_loop_ = true;
				while (!terminated_ && channel_->waitForFrontendMessage()) {
					while (platform::PumpMessageLoop(platform_, isolate_))
					{
					}
				}
				terminated_ = false;
				running_nested_loop_ = false;
			}

			void contextCreated(Local<Context> context, const std::string& name) const {
				std::unique_ptr<v8_inspector::StringBuffer> name_buffer = Utf8ToStringView(name);
				v8_inspector::V8ContextInfo info(context, CONTEXT_GROUP_ID, name_buffer->string());
				client_->contextCreated(info);
			}

			void contextDestroyed(Local<Context> context) const {
				client_->contextDestroyed(context);
			}

			void quitMessageLoopOnPause() override {
				terminated_ = true;
			}

			void connectFrontend(InspectorSessionDelegate* delegate) {
				channel_.reset(RJNEW ChannelImpl(client_.get(), delegate));
			}

			void disconnectFrontend() {
				quitMessageLoopOnPause();
				channel_.reset();
			}

			void dispatchMessageFromFrontend(const v8_inspector::StringView& message) const {
				assert(channel_);
				channel_->dispatchProtocolMessage(message);
			}

			Local<Context> ensureDefaultContextInGroup(int contextGroupId) override {
				(void)contextGroupId;
				return isolate_->GetCurrentContext();
			}

			//TODO: check if this function should be used in RadJAV
			void FatalException(Local<Value> error, Local<Message> message) {
				Local<Context> context = isolate_->GetCurrentContext();

				int script_id = message->GetScriptOrigin().ScriptID()->Value();

				Local<StackTrace> stack_trace = message->GetStackTrace();

				if (!stack_trace.IsEmpty() &&
					stack_trace->GetFrameCount() > 0 &&
					script_id == stack_trace->GetFrame(isolate_, 0)->GetScriptId()) {
					script_id = 0;
				}

				const uint8_t DETAILS[] = "Uncaught";


				client_->exceptionThrown(
					context,
					v8_inspector::StringView(DETAILS, sizeof(DETAILS) - 1),
					error,
					ToProtocolString(isolate_, message->Get())->string(),
					ToProtocolString(isolate_, message->GetScriptResourceName())->string(),
					message->GetLineNumber(context).FromMaybe(0),
					message->GetStartColumn(context).FromMaybe(0),
					client_->createStackTrace(stack_trace),
					script_id);
			}

			ChannelImpl* channel() const {
				return channel_.get();
			}

		private:
			Isolate* isolate_;
			Platform* platform_;
			bool terminated_;
			bool running_nested_loop_;
			std::unique_ptr<v8_inspector::V8Inspector> client_{};
			std::unique_ptr<ChannelImpl> channel_{};
		};

		Agent::Agent(std::string host_name, std::string log_output_path)
			: platform_(nullptr),
			isolate_(nullptr),
			host_name_(host_name),
			log_output_path_(log_output_path)
		{
		}

		Agent::~Agent() = default;

		bool Agent::Start(Isolate *isolate, Platform* platform, RadJAV::String path_to_script) {
			isolate_ = isolate;
			platform_ = platform;
			path_ = path_to_script;

			client_ = std::unique_ptr<CBInspectorClient>(RJNEW CBInspectorClient(isolate_, platform));
			client_->contextCreated(isolate_->GetCurrentContext(), "RadJAV debug context");

			return StartIoThread();
		}

		bool Agent::StartIoThread() {

			if (io_)
			{
				return true;
			}

			io_.reset(new InspectorIo(isolate_, platform_, path_, host_name_, true, log_output_path_, this));

			if (io_->Start())
			{
				return true;
			}

			client_.reset();
			return false;
		}

		void Agent::Stop() {
			if (io_ != nullptr) {
				io_->Stop();
				io_.reset();
			}
		}

		void Agent::Connect(InspectorSessionDelegate* delegate) {
			client_->connectFrontend(delegate);
		}

		bool Agent::IsConnected() const {
			return io_ && io_->IsConnected();
		}

		void Agent::WaitForDisconnect() {
			client_->contextDestroyed(isolate_->GetCurrentContext());
			if (io_ != nullptr) {
				io_->WaitForDisconnect();
			}
		}

		void Agent::FatalException(Local<Value> error, Local<Message> message) {
			if (!IsStarted())
				return;
			client_->FatalException(error, message);
			WaitForDisconnect();
		}

		void Agent::Dispatch(const v8_inspector::StringView& message) {
			client_->dispatchMessageFromFrontend(message);
		}

		void Agent::Disconnect() {
			client_->disconnectFrontend();
		}

		void Agent::RunMessageLoop() {
			client_->runMessageLoopOnPause(CONTEXT_GROUP_ID);
		}

		InspectorSessionDelegate* Agent::delegate() {
			ChannelImpl* channel = client_->channel();
			if (channel == nullptr)
				return nullptr;
			return channel->delegate();
		}

		void Agent::PauseOnNextJavascriptStatement(const RadJAV::String& reason) {
			ChannelImpl* channel = client_->channel();
			if (channel != nullptr)
				channel->schedulePauseOnNextStatement(reason);
		}

	} // namespace inspector
}
#endif

