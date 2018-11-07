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

#include "cpp/RadJavCPPCryptoBase.h"

#include "RadJavString.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#elif defined USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

#include <iostream>
#include <sstream>
#include <iomanip>

#ifdef USE_CRYPTOGRAPHY
	#include <orb/ORB_EngineCrypto.h>

	#include <boost/asio/ssl.hpp>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace Crypto
		{
			#ifdef USE_CRYPTOGRAPHY
			#ifdef USE_V8
			Base::Base(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				//std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
				std::cout << "Args len: " << args.Length() << std::endl;
				
				this -> jsEngine = jsEngine;
				v8::Isolate *isolate = args.GetIsolate();
				
				// Parse common settings
				if (args[0] -> IsObject())
				{
					
					v8::Local<v8::Object> parms = v8::Local<v8::Object>::Cast(args[0]);
					
					myCryptoLibrary = jsEngine -> v8GetString(parms, "cryptoLibrary");
					myAlgorithm = jsEngine -> v8GetString(parms, "algorithm");
					myInputEncoding = jsEngine -> v8GetString(parms, "inputEncoding");
					myOutputEncoding = jsEngine -> v8GetString(parms, "outputEncoding");
				}
				
				// Defaults and error checking
				if (myCryptoLibrary == "") myCryptoLibrary = "OpenSSL"; // TODO - crude way of providing a default
				if (myInputEncoding == "") myInputEncoding = "binary"; // TODO - crude way of providing a default
				if (myAlgorithm == "")
					args.GetIsolate() -> ThrowException(v8::Exception::TypeError
														(v8::String::NewFromUtf8(isolate,
																				 "'algorithm' must be defined")));
				
			}
			
			#elif defined USE_JAVASCRIPTCORE
			
			Base::Base(JSCJavascriptEngine *jsEngine, JSContextRef ctx, RJUINT argumentCount, const JSValueRef arguments[])
			{
				this->jsEngine = jsEngine;
				
				// Parse common settings
				if(argumentCount && JSValueIsObject(ctx, arguments[0]))
				{
					JSObjectRef params = jsEngine->jscCastValueToObject(ctx, arguments[0]);
					
					myCryptoLibrary = jsEngine->jscGetString(params, "cryptoLibrary");
					myAlgorithm = jsEngine->jscGetString(params, "algorithm");
					myInputEncoding = jsEngine->jscGetString(params, "inputEncoding");
					myOutputEncoding = jsEngine->jscGetString(params, "outputEncoding");
				}
				
				// Defaults and error checking
				if (myCryptoLibrary == "") myCryptoLibrary = "OpenSSL"; // TODO - crude way of providing a default
				if (myInputEncoding == "") myInputEncoding = "binary"; // TODO - crude way of providing a default
				if (myAlgorithm == "")
				{
					jsEngine->throwException("'algorithm' must be defined");
				}
			}
			#endif
			
			Base::~Base()
			{

			}

			void Base::processInput(const void *plainText, int textLength,
									std::string &decodedText,
									const void *&binPlainText, int &binPlainTextLength)
			{
				
				String _inputEncoding = myInputEncoding;
				
				//const void *binPlainText;
				//int binPlainTextLength;
				//std::string decodedText;
				
				if (_inputEncoding == "binary")
				{
					binPlainText = plainText;
					binPlainTextLength =  textLength;
				}
				else if (_inputEncoding == "hex")
				{
					auto binData = ORB::Engine::Crypto::decodeHex(plainText, textLength);
					decodedText.assign(static_cast<const char*>(std::get<0>(binData).get()), std::get<1>(binData));
					binPlainText = decodedText.c_str();
					binPlainTextLength = decodedText.length();
				}
				else if (_inputEncoding == "base64")
				{
					auto binData = ORB::Engine::Crypto::decodeBase64(plainText, textLength);
					decodedText.assign(static_cast<const char*>(std::get<0>(binData).get()), std::get<1>(binData));
					binPlainText = decodedText.c_str();
					binPlainTextLength = decodedText.length();
				}
				else
				{
					String msg = "Unsupported input encoding: " + _inputEncoding;
					throw std::invalid_argument(msg);
				}
			}


			void Base::processOutput(std::tuple<std::shared_ptr<void>, unsigned int> result,
									 std::function <void (const std::string& str)> stringSetter,
									 std::function <void (void* buf, int bufLen)> binSetter)
			{
				
				if (myOutputEncoding == "hex")
				{
					stringSetter(ORB::Engine::Crypto::encodeHex(std::get<0>(result).get(),
																std::get<1>(result)));
					return;
				}
				else if (myOutputEncoding == "base64")
				{
					stringSetter(ORB::Engine::Crypto::encodeBase64(std::get<0>(result).get(),
																   std::get<1>(result)));
					return;
				}
				else if (myOutputEncoding == "binary")
				{
					binSetter(std::get<0>(result).get(),
							  std::get<1>(result));
					return;
				}
				String msg = "Unsupported output encoding: " + myOutputEncoding;
				throw std::invalid_argument(msg);
				
			}

			void initializeCertificates()
			{
				DELETEOBJ(SSLCertificates);
				SSLCertificates = RJNEW Array<String> ();
			}

			void addCertificate(String certificate)
			{
				boost::system::error_code err;

				SSLContext.add_certificate_authority(boost::asio::buffer(certificate.c_str(), certificate.size()), err);
				SSLCertificates->push_back(certificate);

				/// @fixme Throw an error if err comes back with something.
			}

			String getCertificate(RJINT index)
			{
				return (SSLCertificates->at (index));
			}

			Array<String> *getCertificates()
			{
				return (RadJAV::CPP::Crypto::SSLCertificates);
			}

			Array<String> getDefaultCertificates()
			{
				Array<String> certificates;

				certificates.push_back ("\n\
								/// DigiCert High Assurance EV Root CA\n\
								-----BEGIN CERTIFICATE-----\n\
								MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n\
								MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n\
								d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n\
								ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n\
								MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n\
								LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n\
								RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n\
								+ 9S75S0tMqbf5YE / yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n\
								PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n\
								xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n\
								Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx + mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n\
								hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n\
								EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH / BAQDAgGGMA8GA1UdEwEB / wQF\n\
								MAMBAf8wHQYDVR0OBBYEFLE + w2kD + L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n\
								FLE + w2kD + L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n\
								nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe / EW1ntlMMUu4kehDLI6z\n\
								eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n\
								hS9OMPagMRYjyOfiZRYzy78aG6A9 + MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n\
								Yzi9RKR / 5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2 / S6cCZdkGCe\n\
								vEsXCS + 0yx5DaMkHJ8HSXPfqIbloEpw8nL + e / IBcm2PN7EeqJSdnoDfzAIJ9VNep\n\
								+ OkuE6N36B9K\n\
								----- END CERTIFICATE-----\n");
				
				certificates.push_back("\n\
								/// GeoTrust Global CA\n\
								-----BEGIN CERTIFICATE-----\n\
								MIIDVDCCAjygAwIBAgIDAjRWMA0GCSqGSIb3DQEBBQUAMEIxCzAJBgNVBAYTAlVT\n\
								MRYwFAYDVQQKEw1HZW9UcnVzdCBJbmMuMRswGQYDVQQDExJHZW9UcnVzdCBHbG9i\n\
								YWwgQ0EwHhcNMDIwNTIxMDQwMDAwWhcNMjIwNTIxMDQwMDAwWjBCMQswCQYDVQQG\n\
								EwJVUzEWMBQGA1UEChMNR2VvVHJ1c3QgSW5jLjEbMBkGA1UEAxMSR2VvVHJ1c3Qg\n\
								R2xvYmFsIENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2swYYzD9\n\
								9BcjGlZ + W988bDjkcbd4kdS8odhM + KhDtgPpTSEHCIjaWC9mOSm9BXiLnTjoBbdq\n\
								fnGk5sRgprDvgOSJKA + eJdbtg / OtppHHmMlCGDUUna2YRpIuT8rxh0PBFpVXLVDv\n\
								iS2Aelet8u5fa9IAjbkU + BQVNdnARqN7csiRv8lVK83Qlz6cJmTM386DGXHKTubU\n\
								1XupGc1V3sjs0l44U + VcT4wt / lAjNvxm5suOpDkZALeVAjmRCw7 + OC7RHQWa9k0 +\n\
								bw8HHa8sHo9gOeL6NlMTOdReJivbPagUvTLrGAMoUgRx5aszPeE4uwc2hGKceeoW\n\
								MPRfwCvocWvk + QIDAQABo1MwUTAPBgNVHRMBAf8EBTADAQH / MB0GA1UdDgQWBBTA\n\
								ephojYn7qwVkDBF9qn1luMrMTjAfBgNVHSMEGDAWgBTAephojYn7qwVkDBF9qn1l\n\
								uMrMTjANBgkqhkiG9w0BAQUFAAOCAQEANeMpauUvXVSOKVCUn5kaFOSPeCpilKIn\n\
								Z57QzxpeR + nBsqTP3UEaBU6bS + 5Kb1VSsyShNwrrZHYqLizz / Tt1kL / 6cdjHPTfS\n\
								tQWVYrmm3ok9Nns4d0iXrKYgjy6myQzCsplFAMfOEVEiIuCl6rYVSAlk6l5PdPcF\n\
								PseKUgzbFbS9bZvlxrFUaKnjaZC2mqUPuLk / IH2uSrW4nOQdtqvmlKXBx4Ot2 / Un\n\
								hw4EbNX / 3aBd7YdStysVAq45pmp06drE57xNNB6pXE0zX5IJL4hmXXeXxx12E6nV\n\
								5fEWCRE11azbJHFwLJhWC9kXtNHjUStedejV0NxPNO3CBWaAocvmMw ==\n\
								---- - END CERTIFICATE---- - \n");

				return (certificates);
			}
		  #endif
		}
	}
}

