/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#include "v8/RadJavV8NetWebSocketSsl.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPNetWebSocketServerSsl.h"
#include "cpp/RadJavCPPNetWebSocketClientSsl.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace Net
		{
			void WebSocketServerSsl::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_init", WebSocketServerSsl::_init);

				V8_CALLBACK(object, "listen", WebSocketServerSsl::listen);
				V8_CALLBACK(object, "sendToAll", WebSocketServerSsl::sendToAll);
				V8_CALLBACK(object, "send", WebSocketServerSsl::send);
				V8_CALLBACK(object, "receive", WebSocketServerSsl::receive);
				V8_CALLBACK(object, "close", WebSocketServerSsl::close);

				V8_CALLBACK(object, "onReceive", WebSocketServerSsl::onReceive);
				V8_CALLBACK(object, "onAccept", WebSocketServerSsl::onAccept);

			}

			void WebSocketServerSsl::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			  
				v8::Isolate *isolate = args.GetIsolate();
				std::map<std::string, std::string> parms;
				//parms["certificateStore"]
				parms["certificateStoreType"] = "pem";
				//parms["keyStore"}
				parms["keyStorePwd"] = "";
				parms["[keyStoreType"] = "pem";

				
				if (args[0] -> IsString() && args[1] -> IsString())
				  {
				    parms["certificateStore"] =  parseV8Value(args[0]);
				    parms["keyStore"] = parseV8Value(args[1]);
				    if (args[2] -> IsString())
				      parms["keyStorePwd"] = parseV8Value(args[2]);
				  }

				else if (args[0] -> IsObject())
				  {

				    v8::Local<v8::Object> parmsObj = v8::Local<v8::Object>::Cast(args[0]);

				    parms["certificateStore"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "certificateStore");
				    parms["certificateStoreType"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "certificateStoreType");

				    parms["keyStore"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "keyStore");				    
				    parms["keyStoreType"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "keyStoreType");
				    parms["keyStorePwd"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "keyStorePwd");

				    parms["trustStore"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStore");				    
				    parms["trustStoreType"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStoreType");
				    parms["trustkeyStorePwd"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStorePwd");

				    parms["verifyMode"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "verifyMode");				    
				    

				  }

				for (auto elem: {"certificateStore", "keyStore"})
				  {
				    if (parms.find(elem) == parms.end() || parms[elem] == "")
					args.GetIsolate() -> ThrowException(v8::Exception::TypeError
									    (v8::String::NewFromUtf8(isolate,
												     (std::string("'") + elem + "' must be defined").c_str())));
				  }
				
				if (parms.find("certificateStoreType") == parms.end() || parms["certificateStoreType"] == "")
				  parms["certificateStoreType"] = "pem";
				if (parms.find("keyStoreType") == parms.end() || parms["keyStoreType"] == "")
				  parms["keyStoreType"] = "pem";
				if (parms.find("keyStorePwd") == parms.end())
				  parms["keyStorePwd"] = "";

				

			  
				CPP::Net::WebSocketServerSsl *webSocket = RJNEW CPP::Net::WebSocketServerSsl(parms);
				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_webSocket", webSocket);
			}

			void WebSocketServerSsl::listen(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServerSsl * webSocket = (CPP::Net::WebSocketServerSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				
				v8::Local<v8::Number> port;
				RJINT portI = 0;

				if (args.Length() > 0)
				{
					port = v8::Local<v8::Number>::Cast(args[0]);
					portI = port->IntegerValue();
					V8_JAVASCRIPT_ENGINE->v8SetNumber(args.This(), "port", portI);
				}

				webSocket->listen(portI);
			}

			void WebSocketServerSsl::sendToAll(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServerSsl * webSocket = (CPP::Net::WebSocketServerSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String str = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, val);

				webSocket->sendToAll(str);
			}

			void WebSocketServerSsl::send(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServerSsl *webSocket = (CPP::Net::WebSocketServerSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Isolate *isolate = args.GetIsolate();
				
				v8::Local<v8::String> val_id = v8::Local<v8::String>::Cast(args[0]);
				String id = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, val_id);


				if (args[1] -> IsString())
				  {
				    v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[1]);
				    String message = parseV8Value(val);

				    webSocket->send(id, message);
				  }
				else if (args[1] -> IsObject())
				  {
				    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[1]) -> GetConstructorName());
				    if (constructor.find("Array") != std::string::npos)
				      {
					auto message = v8::Local<v8::ArrayBuffer>::Cast(args[1]);
					const void *msgData;
					int msgLen;
					msgData = message -> GetContents().Data();
					msgLen = message -> ByteLength();
					webSocket->send(id, msgData, msgLen);
				      }
				    else
				      isolate -> ThrowException(v8::Exception::TypeError
								(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));

				  }
				
			}

			void WebSocketServerSsl::receive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServerSsl *webSocket = (CPP::Net::WebSocketServerSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				auto resp = webSocket->receive();
				v8::Local<v8::Value> ret;
				v8::Isolate *isolate = args.GetIsolate();

				ret = v8::String::NewFromUtf8(isolate, resp.c_str());
				args.GetReturnValue().Set(ret);
			}

			void WebSocketServerSsl::close(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServerSsl *webSocket = (CPP::Net::WebSocketServerSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				
				webSocket->close();
			}

			void WebSocketServerSsl::onAccept(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServerSsl *webSocket = (CPP::Net::WebSocketServerSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Persistent<v8::Function> *func = RJNEW v8::Persistent<v8::Function>();
				v8::Local<v8::Function> newEvt = v8::Local<v8::Function>::Cast(args[0]);

				func->Reset(V8_JAVASCRIPT_ENGINE->isolate, newEvt);

				webSocket -> set_on_accept_callback(func);
			}

			void WebSocketServerSsl::onReceive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServerSsl *webSocket = (CPP::Net::WebSocketServerSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Persistent<v8::Function> *func = RJNEW v8::Persistent<v8::Function>();
				v8::Local<v8::Function> newEvt = v8::Local<v8::Function>::Cast(args[0]);

				func->Reset(V8_JAVASCRIPT_ENGINE->isolate, newEvt);
				
				webSocket -> set_on_receive_callback(func);
			}


			void WebSocketClientSsl::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{

			  std::cout << __PRETTY_FUNCTION__ << std::endl;
			  
				v8::Isolate *isolate = args.GetIsolate();
				std::map<std::string, std::string> parms;
				//parms["certificateStore"]
				parms["certificateStoreType"] = "pem";
				//parms["keyStore"}
				parms["keyStorePwd"] = "";
				parms["[keyStoreType"] = "pem";

				//parms["trustStore"}
				parms["trustStorePwd"] = "";
				parms["[trustStoreType"] = "pem";
				
				
				if (args[0] -> IsString() && args[1] -> IsString())
				  {
				    parms["trustStore"] =  parseV8Value(args[0]);
				    parms["trustStorePwd"] = parseV8Value(args[1]);
				  }

				else if (args[0] -> IsObject())
				  {

				    v8::Local<v8::Object> parmsObj = v8::Local<v8::Object>::Cast(args[0]);

				    parms["certificateStore"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "certificateStore");
				    parms["certificateStoreType"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "certificateStoreType");

				    parms["keyStore"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "keyStore");				    
				    parms["keyStoreType"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "keyStoreType");
				    parms["keyStorePwd"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "keyStorePwd");

				    parms["trustStore"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStore");				    
				    parms["trustStoreType"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStoreType");
				    parms["trustkeyStorePwd"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStorePwd");
				    
				    parms["verifyMode"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "verifyMode");				    
				  }

				for (auto elem: {"trustStore"})
				  {
				    if (parms.find(elem) == parms.end() || parms[elem] == "")
					args.GetIsolate() -> ThrowException(v8::Exception::TypeError
									    (v8::String::NewFromUtf8(isolate,
												     (std::string("'") + elem + "' must be defined").c_str())));
				  }

				/*
				if (parms.find("certificateStoreType") == parms.end() || parms["certificateStoreType"] == "")
				  parms["certificateStoreType"] = "pem";
				if (parms.find("keyStoreType") == parms.end() || parms["keyStoreType"] == "")
				  parms["keyStoreType"] = "pem";
				if (parms.find("keyStorePwd") == parms.end())
				  parms["keyStorePwd"] = "";

				*/			  

				CPP::Net::WebSocketClientSsl *webSocket = RJNEW CPP::Net::WebSocketClientSsl(parms);
				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_webSocket", webSocket);




				
			}

			void WebSocketClientSsl::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_init", WebSocketClientSsl::_init);

				V8_CALLBACK(object, "connect", WebSocketClientSsl::connect);
				V8_CALLBACK(object, "send", WebSocketClientSsl::send);
				V8_CALLBACK(object, "receive", WebSocketClientSsl::receive);
				V8_CALLBACK(object, "close", WebSocketClientSsl::close);

			}

			void WebSocketClientSsl::connect(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClientSsl *webSocket = (CPP::Net::WebSocketClientSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String host = parseV8Value(val);
				val = v8::Local<v8::String>::Cast(args[1]);
				String port = parseV8Value(val);
				std::cout  << __PRETTY_FUNCTION__ << ": about to connect" << std::endl << std::flush;

				webSocket->connect(host, port);
			}

			void WebSocketClientSsl::send(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClientSsl *webSocket = (CPP::Net::WebSocketClientSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				v8::Isolate *isolate = args.GetIsolate();

				if (args[0] -> IsString())
				  {
				    v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				    String message = parseV8Value(val);

				    webSocket->send(message);
				  }
				else if (args[0] -> IsObject())
				  {
				    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[0]) -> GetConstructorName());
				    if (constructor.find("Array") != std::string::npos)
				      {
					auto message = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
					const void *msgData;
					int msgLen;
					msgData = message -> GetContents().Data();
					msgLen = message -> ByteLength();
					webSocket->send(msgData, msgLen);
				      }
				    else
				      isolate -> ThrowException(v8::Exception::TypeError
								(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));

				  }
			}

			void WebSocketClientSsl::receive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClientSsl *webSocket = (CPP::Net::WebSocketClientSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				//auto resp = webSocket->receive();
				v8::Local<v8::Value> ret;
				v8::Isolate *isolate = args.GetIsolate();

				webSocket->receive([&ret, isolate](const std::string &str)
						   {
						     ret = v8::String::NewFromUtf8(isolate,
										   str.c_str());
						   },
						   [&ret, isolate](const void* bufPtr, int bufLen)
						   {
						     auto ab = v8::ArrayBuffer::New(isolate, bufLen);
						     std::memcpy(ab -> GetContents().Data(), bufPtr, bufLen);
						     ret = ab;
						   }
						   );
						     
				
				//ret = v8::String::NewFromUtf8(isolate, resp.c_str());
				args.GetReturnValue().Set(ret);
			}

			void WebSocketClientSsl::close(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClientSsl *webSocket = (CPP::Net::WebSocketClientSsl *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				webSocket->close();
			}

		}
	}
}
#endif

