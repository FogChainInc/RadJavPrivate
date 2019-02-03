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
#include "v8/RadJavV8NetUdpClient.h"

#include "RadJav.h"

#include "cpp/RadJavCPPNetUdpClient.h"
#define ENGINE CPP::Net::UdpClient


#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"


namespace RadJAV
{
	namespace V8B
	{
		namespace Net
		{
			void UdpClient::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
			  //std::cout << __PRETTY_FUNCTION__ << std::endl;
				V8_CALLBACK(object, "_init", UdpClient::_init);

				V8_CALLBACK(object, "connect", UdpClient::connect);
				V8_CALLBACK(object, "send", UdpClient::send);
				V8_CALLBACK(object, "receive", UdpClient::receive);
				V8_CALLBACK(object, "close", UdpClient::close);
				
			} // End of createV8Callbacks

		        void UdpClient::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
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

				     parms["maxDatagramSize"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "maxDatagramSize");
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
				   V8_JAVASCRIPT_ENGINE -> v8SetExternal(args.This(), "_engine", engine);
				 }
				 catch (std::exception &e)
				   {

				     isolate -> ThrowException(v8::Exception::TypeError
							       (v8::String::NewFromUtf8(isolate,
											e.what())));
				   }
				
			} // End of _init

			void UdpClient::connect(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			        // JavasScript args
				const int HOST = 0;
				const int PORT = 1;
			  
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE -> v8GetExternal<ENGINE>(args.This(), "_engine");
				v8::Isolate *isolate = args.GetIsolate();

				String host = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
								  v8::Local<v8::String>::Cast(args[HOST]));
				
				String port = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
								  v8::Local<v8::String>::Cast(args[PORT]));

				try
				  {
				    engine -> connect(host, port);
				  }
				catch (std::exception &e)
				  {
				    isolate -> ThrowException(v8::Exception::TypeError
							      (v8::String::NewFromUtf8(isolate,
										       e.what())));
				  }

				
			} // End of connect

			void UdpClient::send(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			        // JavasScript args
			        const int MSG = 0;
				const int HOST = 1;
				const int PORT = 2;
				
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE -> v8GetExternal<ENGINE>(args.This(), "_engine");
				v8::Isolate *isolate = args.GetIsolate();

				//std::cout << __PRETTY_FUNCTION__ << std::endl;
				//std::cout << "Args : " << args.Length() << std::endl;

				try
				  {
				    // Implements send(message)
				    if (args.Length() == 1)   
				      {
					if (args[MSG] -> IsString())
					  {
					    v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[MSG]);
					    String message = parseV8Value(val);

					    engine -> send(message);
					  }
					else if (args[MSG] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[MSG]) -> GetConstructorName());
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto message = v8::Local<v8::ArrayBuffer>::Cast(args[MSG]);
						const void *msgData;
						int msgLen;
						msgData = message -> GetContents().Data();
						msgLen = message -> ByteLength();
						engine -> send(msgData, msgLen);
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					
					  }
					else
					  {
					    std::string message = "Wrong message object type, only string and ArrayBuffer are supported";
					    isolate -> ThrowException(v8::Exception::TypeError
								      (v8::String::NewFromUtf8(isolate,
											       message.c_str())));

					  }
				      }
					
				    // Implements send(message, host, service)
				    else if (args.Length() == 3) 
				      {

					String host = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									  v8::Local<v8::String>::Cast(args[HOST]));

					String port = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									  v8::Local<v8::String>::Cast(args[PORT]));

					if (args[MSG] -> IsString())
					  {
					    v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[MSG]);
					    String message = parseV8Value(val);

					    engine -> send(message, host, port);
					  }
					else if (args[MSG] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[MSG]) -> GetConstructorName());
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto message = v8::Local<v8::ArrayBuffer>::Cast(args[MSG]);
						const void *msgData;
						int msgLen;
						msgData = message -> GetContents().Data();
						msgLen = message -> ByteLength();
						engine -> send(msgData, msgLen, host, port);
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					
					  }
					else
					  {
					    std::string message = "Wrong message object type, only string and ArrayBuffer are supported";
					    isolate -> ThrowException(v8::Exception::TypeError
								      (v8::String::NewFromUtf8(isolate,
											       message.c_str())));
					  }
				      }

				    // Any other number of arguments than one or 3 throws an error
				    else
				      {
					std::string message = "Wrong number of parameters" + std::to_string(args.Length()) + ", expecting message, or message, host and service";
					isolate -> ThrowException(v8::Exception::TypeError
								  (v8::String::NewFromUtf8(isolate,
											   message.c_str())));
				      }
				  }

				// Catch whatever C++ layer threw and convert to V8 exception.
				catch (std::exception &e)
				  {
				    isolate -> ThrowException(v8::Exception::TypeError
							      (v8::String::NewFromUtf8(isolate,
										       e.what())));
				  }
				 
			} // End of send

			void UdpClient::receive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			        // JavasScript args
				const int HOST = 0;
				const int PORT = 1;

				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE -> v8GetExternal<ENGINE>(args.This(), "_engine");

				//auto resp = engine -> receive();
				v8::Local<v8::Value> ret;
				v8::Isolate *isolate = args.GetIsolate();

				//std::cout << __PRETTY_FUNCTION__ << std::endl;
				//std::cout << "Args : " << args.Length() << std::endl;

				try
				  {
				    // Implements var result = receive();
				    if (args.Length() == 0)
				      {
					engine -> receive([&ret, isolate](const void *bufPtr, int bufLen)
							  {
							    auto ab = v8::ArrayBuffer::New(isolate, bufLen);
							    std::memcpy(ab -> GetContents().Data(), bufPtr, bufLen);
							    ret = ab;
							  }
							  );
				      }

				    // Implements var result = receive(host, service);
				    else if (args.Length() == 2)
				      {

					String host = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									  v8::Local<v8::String>::Cast(args[HOST]));

					String service = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE -> isolate,
									     v8::Local<v8::String>::Cast(args[PORT]));


					engine -> receive([&ret, isolate](const void *bufPtr, int bufLen)
							  {
							    auto ab = v8::ArrayBuffer::New(isolate, bufLen);
							    std::memcpy(ab -> GetContents().Data(), bufPtr, bufLen);
							    ret = ab;
							  },
							  host, service
							  );
					
				      }

				    // Any other number of arguments than none or 2 throws an error
				    else
				      {
					std::string message = "Wrong number of parameters" + std::to_string(args.Length()) + ", expecting none or 2 (host and port)";
					isolate -> ThrowException(v8::Exception::TypeError
								  (v8::String::NewFromUtf8(isolate,
											   message.c_str())));

				      }
				
				    //ret = v8::String::NewFromUtf8(isolate, resp.c_str());
				    args.GetReturnValue().Set(ret);

				  }
				
				// Catch whatever C++ layer threw and convert to V8 exception.
				catch (std::exception &e)
				  {
				    isolate -> ThrowException(v8::Exception::TypeError
							      (v8::String::NewFromUtf8(isolate,
										       e.what())));
				  }
				
			} // End of receive

			void UdpClient::close(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE -> v8GetExternal<ENGINE>(args.This(), "_engine");

				engine -> close();
				
			} // End of close

		}
	}
}
#endif

