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
#ifndef _RADJAV_CPP_AGENT_H_
#define _RADJAV_CPP_AGENT_H_

#ifdef USE_INSPECTOR
#include "RadJav.h"
#include <string>
#include "v8.h"
#include "v8-inspector.h"
#include "RadJavCPPNetWebServerUpgradable.h"

namespace inspector {

	using namespace v8;
	
	std::unique_ptr<v8_inspector::StringBuffer> Utf8ToStringView(const std::string& message);

	class CBInspectorClient;
	class InspectorIo;

	class InspectorSessionDelegate {
	public:
		virtual ~InspectorSessionDelegate() = default;
		virtual bool WaitForFrontendMessageWhilePaused() = 0;
		virtual void SendMessageToFrontend(const v8_inspector::StringView& message) = 0;
	};

	class Agent {
	public:
		Agent(std::string host_name, std::string log_output_path);
		~Agent();

		bool Start(Isolate* isolate, Platform* platform, RadJAV::String path_to_script);
		
		void Stop();

		bool IsStarted() const { return !!client_; }

		// IO thread started, and client connected
		bool IsConnected() const;
		
		void WaitForDisconnect();

		void FatalException(Local<Value> error,	v8::Local<v8::Message> message);

		// These methods are called by the WS protocol and JS binding to create
		// inspector sessions.  The inspector responds by using the delegate to send
		// messages back.
		void Connect(InspectorSessionDelegate* delegate);
		void Disconnect();
		void Dispatch(const v8_inspector::StringView& message);
		
		InspectorSessionDelegate* delegate();

		void RunMessageLoop();
		
		void PauseOnNextJavascriptStatement(const RadJAV::String& reason);

		InspectorIo* io() {
			return io_.get();
		}

		// Can only be called from the the main thread.
		bool StartIoThread();

	private:
		std::unique_ptr<CBInspectorClient> client_{};
		std::unique_ptr<InspectorIo> io_{};
		Platform* platform_;
		Isolate * isolate_;
		RadJAV::String path_;
		RadJAV::String host_name_;
		RadJAV::String log_output_path_;
	};

}  // namespace inspector

#endif
#endif  // _RADJAV_CPP_AGENT_H_
