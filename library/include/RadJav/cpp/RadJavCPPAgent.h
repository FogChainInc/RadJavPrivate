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

    #ifdef USE_V8
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
            /**
             * @class InspectorSessionDelegate
             *
             * @ingroup group_debug
             *
             * @brief V8 Inspector session delegate class for waiting and sending message to frontend part of debugger
             *
             * This prototype class is meant to be used only by the v8 Inspector Agent, its implementation
             * is not complete and only serves as connection point for Chrome Dev Tools with
             * minimal functionality to allow JS debugging.
             *
             */
            class InspectorSessionDelegate {
            public:
                virtual ~InspectorSessionDelegate() = default;
                virtual bool WaitForFrontendMessageWhilePaused() = 0;
                virtual void SendMessageToFrontend(const v8_inspector::StringView& message) = 0;
            };
            /**
             * @class Agent
             *
             * @ingroup group_debug
             *
             * @brief V8 Inspector Agent class
             *
             * This class is meant to be used only by the v8 Inspector code, its implementation
             * is not complete and only serves as connection point for Chrome Dev Tools with
             * minimal functionality to allow JS debugging.
             *
             */
            class Agent {
            public:
                /**
                 * @brief Create agent class
                 *
                 * usage:
                 * #ifdef USE_INSPECTOR
				 * if (useInspector) {
				 * 	agent_ = RJNEW inspector::Agent("0.0.0.0", "inspector.log");
				 * 	agent_->Start(isolate, platform, fileName);
				 * 	agent_->PauseOnNextJavascriptStatement("Break on start");
				 * }
			     * #endif
                 *
                 * @param host_name address to listen
                 *
                 * @param log_output_path string with log file name
                 */
                Agent(std::string host_name, std::string log_output_path);
                ~Agent();

                /**
                 * @brief Initialize agent and start listening to incoming connection from external debugger
                 *
                 * @param isolate V8 isolate under debug
                 *
                 * @param platform V8 platform under debug
                 *
                 * @param path_to_script name of the script to debug with full path. This value is used to
                 * present unit for testing in Chrome Dev tools
                 *
                 * @return returns true if successful
                 */
                bool Start(Isolate* isolate, Platform* platform, RadJAV::String path_to_script);

                /**
                 * @brief Stop agent
                 */
                void Stop();

                /**
                 * @brief Check if debug client exists
                 *
                 * @return true if client exists
                 */
                bool IsStarted() const { return !!client_; }

                 /**
                 * @brief Check if IO thread started and client connected
                 *
                 * @return true if IO thread started and client connected
                 */
                bool IsConnected() const;

                /**
                 * @brief Wait for disconnect
                 */
                void WaitForDisconnect();

                /**
                 * @brief Dispatch fatal exception
                 *
                 * @param error V8 local value with error
                 *
                 * @param message V8 exception message
                 *
                 */
                void FatalException(Local<Value> error, v8::Local<v8::Message> message);

                /**
                 * @brief Connect Inspector and CDT with InspectorSessionDelegate
                 *
                 * Method called by the WS protocol and JS binding to create inspector sessions.
                 * The inspector responds by using the delegate to send messages back.
                 *
                 * @param delegate communication delegate
                 */
                void Connect(InspectorSessionDelegate* delegate);

                /**
                 * @brief Disconnect Inspector and CDT
                 *
                 * Method called by the WS protocol and JS binding to release inspector sessions.
                 *
                 */
                void Disconnect();

                /**
                 * @brief Dispatch message between Inspector and CDT
                 *
                 * Method called by the WS protocol and JS binding to process message of inspector session.
                 *
                 * @param message StringView& message to dispatch
                 */
                void Dispatch(const v8_inspector::StringView& message);

                /**
                 * @brief Get Inspector delegate
                 *
                 * Method called by the WS protocol and JS binding to process inspector session.
                 *
                 * @return pointer to session delegate
                 *
                 */
                InspectorSessionDelegate* delegate();

                /**
                 * @brief Proceed with V8 message loop
                 */
                void RunMessageLoop();


                /**
                 * @brief Set V8 debugger to pause on next JS statement
                 *
                 * Pause reason appears in the logs, useful to stop at the beginning of debug session
                 *
                 * @param reason Pause reason that appears in the logs
                 */
                void PauseOnNextJavascriptStatement(const RadJAV::String& reason);

                /**
                 * @brief Get Inspector IO object pointer
                 *
                 * @return Inspector IO object pointer
                 */
                InspectorIo* io() {
                    return io_.get();
                }

                /**
                 * @brief Start IO thread of Agent
                 *
                 * Can only be called from the the main thread.
                 *
                 * @return success
                 */
                bool StartIoThread();

            private:
                /**
                 * @brief pointer to Inspector Client object
                 */
                std::unique_ptr<CBInspectorClient> client_{};

                /**
                 * @brief Pointer to Inspector IO object
                 */
                std::unique_ptr<InspectorIo> io_{};

                /**
                 * @brief Pointer to V8 Platform object
                 */
                Platform* platform_;

                /**
                 * @brief Pointer to V8 Isolate object
                 */
                Isolate * isolate_;

                /**
                 * @brief path to script under debug
                 */
                RadJAV::String path_;

                /**
                 * @brief host name Chrome Dev tools will connect to (0.0.0.0)
                 */
                RadJAV::String host_name_;

                /**
                 * @brief path to log file used by inspector
                 */
                RadJAV::String log_output_path_;
            };

        }  // namespace inspector

        #endif
    #endif
#endif  // _RADJAV_CPP_AGENT_H_
