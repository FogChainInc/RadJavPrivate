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
#include "v8/RadJavV8NetUdpServer.h"

#include "RadJav.h"

#include "cpp/RadJavCPPNetUdpServer.h"
#include "cpp/RadJavCPPNetUdpClient.h"

#define ENGINE CPP::Net::UdpServer


#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"


namespace RadJAV
{
	namespace V8B
	{
		namespace Net
		{
			void UdpServer::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{

			  std::cout << "JAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl << std::flush;
				V8_CALLBACK(object, "_init", UdpServer::_init);

				V8_CALLBACK(object, "listen", UdpServer::listen);
				V8_CALLBACK(object, "send", UdpServer::send);
				V8_CALLBACK(object, "close", UdpServer::close);

				V8_CALLBACK(object, "onReceive", UdpServer::onReceive);
				V8_CALLBACK(object, "on", UdpServer::on);

			  std::cout << "JAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA2" << std::endl << std::flush;
				
			} // End of createV8Callbacks

			void UdpServer::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				 v8::Isolate *isolate = args.GetIsolate();
				 std::map<std::string, std::string> parms;

				 std::cout << "Args LEN:" << args.Length() << std::endl;

				 if (args.Length() == 0)
				   {
				   }
				 else if (args.Length() == 1 && args[0] -> IsObject())
				   {
					
				     v8::Local<v8::Object> parmsObj = v8::Local<v8::Object>::Cast(args[0]);


				     parms["threads"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "threads");
				     std::cout << "Threadsssssssss: " << parms["threads"] << std::endl;
				     
				     parms["maxDatagramSize"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "maxDatagramSize");
				     parms["fastServerMode"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "fastServerMode");
				   }
				 else
				   {
				     String msg = "Unsupported constructor";
				     isolate -> ThrowException(v8::Exception::TypeError
							       (v8::String::NewFromUtf8(isolate,
											msg.c_str())));
				   }

				 try {

				   std::shared_ptr<ENGINE> engine(RJNEW ENGINE(parms), [](ENGINE* p){DELETEOBJ(p)});
				   V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_engine", engine);
				 }
				 catch (std::exception &e)
				   {

				     isolate -> ThrowException(v8::Exception::TypeError
							       (v8::String::NewFromUtf8(isolate,
											e.what())));
				   }
			  
			} // End of _init

			void UdpServer::listen(const v8::FunctionCallbackInfo<v8::Value> &args)
			{

				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
				v8::Isolate *isolate = args.GetIsolate();
				

				if (args.Length() == 2)
				{

					String host = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									  v8::Local<v8::String>::Cast(args[0]));
					String port = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									  v8::Local<v8::String>::Cast(args[1]));
					
					engine -> listen(host, port);
				}
				else
				  {
				    std::string message = "Wrong number of parameters" + std::to_string(args.Length()) + ", expecting 2, host and port";
				    isolate -> ThrowException(v8::Exception::TypeError
							      (v8::String::NewFromUtf8(isolate,
										       message.c_str())));

				  }


			} // End of listen

			void UdpServer::send(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			        // Args from JavaScript
				const int MSG = 0;
				const int HOST = 1;
				const int PORT = 2;
			  
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
				v8::Isolate *isolate = args.GetIsolate();
				
				String recipientAddr = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									   v8::Local<v8::String>::Cast(args[HOST]));

				String recipientService = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									      v8::Local<v8::String>::Cast(args[PORT]));
				
				std::cout << "Sending to: " << recipientAddr << ":" << recipientService << std::endl;

				try {
				  if (args[MSG] -> IsString())
				    {
				      String message = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									   v8::Local<v8::String>::Cast(args[MSG]));
				      std::cout << "IsString: " << message << std::endl;
				      engine -> send(message, recipientAddr, recipientService);
				    }
				  else if (args[MSG] -> IsObject())
				    {
				      String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[MSG]) -> GetConstructorName());
				      std::cout << "IsObject: " << constructor << std::endl;
				    
				      if (constructor.find("Array") != std::string::npos)
					{
					  std::cout << "It is array" << std::endl;
					
					  auto message = v8::Local<v8::ArrayBuffer>::Cast(args[MSG]);
					  const void *msgData;
					  int msgLen;
					  msgData = message -> GetContents().Data();
					  msgLen = message -> ByteLength();
					  std::cout.write((char*)msgData, msgLen);
					  std::cout << std::endl;
					
					  engine -> send(msgData, msgLen, recipientAddr, recipientService);
					}
				      else
					isolate -> ThrowException(v8::Exception::TypeError
								  (v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
				    }
				  else
				    isolate -> ThrowException(v8::Exception::TypeError
							      (v8::String::NewFromUtf8(isolate, "Unsupported argument type")));
				}
				catch (std::exception &e)
				  {

				    isolate -> ThrowException(v8::Exception::TypeError
							      (v8::String::NewFromUtf8(isolate,
										       e.what())));
				  }
			} // End of send

			void UdpServer::onReceive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");

				v8::Isolate *isolate = args.GetIsolate();
				
				v8::Persistent<v8::Function> *func = RJNEW v8::Persistent<v8::Function>();
				v8::Local<v8::Function> newEvt = v8::Local<v8::Function>::Cast(args[0]);

				func -> Reset(V8_JAVASCRIPT_ENGINE -> isolate, newEvt);
				
				engine -> setOnReceiveCallback([isolate, func](std::shared_ptr<const unsigned char> buf, int bytes, const std::string &senderAddr, unsigned short port)
							       {
								 // Args from JavaScript
								 const int MSG = 0;
								 const int HOST = 1;
								 const int PORT = 2;
								 const int ARGS = 3;
								 
								 std::cout << "Received " << bytes << " bytes of data from " << senderAddr << ":" << port << std::endl;

								 v8::Locker myLocker(isolate);
								 isolate -> Enter();					  

								 v8::Local<v8::Function> evt =
								   v8::Local<v8::Function>::Cast(func -> Get(isolate));
						
								 v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[ARGS];

								 auto message = v8::ArrayBuffer::New(isolate, bytes);
								 std::memcpy(message -> GetContents().Data(), buf.get(), bytes);
								 args[MSG] = message;

								 String v8Sender(senderAddr);

								 args[HOST] = v8Sender.toV8String(isolate);

								 v8::Local<v8::Integer> v8Port = v8::Integer::New(isolate, port);
								 args[PORT] = v8Port;
						
								 if (V8_JAVASCRIPT_ENGINE -> v8IsNull(evt) == false)
								   evt -> Call(V8_JAVASCRIPT_ENGINE -> globalContext -> Global(), ARGS, args);
								    
								 DELETE_ARRAY(args);
								 isolate -> Exit();					  
								    
							       }
							       );
			} // End of onReceive

		        void UdpServer::close(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
				
				engine -> close();
			}
		  

		        // Need to wire this one eventually
			void UdpServer::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
				
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);


				//engine -> on(event, func);
			} // End of on

		}
	}
}
#endif

