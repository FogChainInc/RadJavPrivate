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

/// <reference path="RadJav.ts" />
/// <reference path="RadJav.Vector2.ts" />
/// <reference path="RadJav.Net.WebServer.ts" />
/// <reference path="RadJav.Net.WebSocketConnection.ts" />
/// <reference path="RadJav.IO.ts" />

namespace RadJav
{
	/** @class RadJav.Testing
	* The testing class.
	* Available on platforms: Windows,Linux,OSX,iOS,Android
	*/
	export namespace Testing
	{
		/// The testing library.
		export class TestLibrary
		{
			test: Test;

			constructor (test: Test)
			{
				this.test = test;
			}

			/// Show a successful message. 
			success (message: string = ""): void
			{
				this.test.passed.push (true);
				this.test.results.push (message);
			}

			/// Throw an error message. 
			error (message: string = "", expected: any = undefined, result: any = undefined): void
			{
				this.test.passed.push (false);
				this.test.results.push (message);
			}

			/// Test an expression.
			assert (expression: boolean, errorMessage: string = ""): void
			{
				if (expression == true)
					this.success (errorMessage);
				else
					this.error (errorMessage);
			}

			/// Checks if the objects are equal.
			equal (expected: any, result: any, message: string = ""): void
			{
				if (expected == result)
					this.success (message);
				else
					this.error (message);
			}

			/// Checks if the objects are not equal.
			notEqual (expected: any, result: any, message: string = ""): void
			{
				if (expected != result)
					this.success (message);
				else
					this.error (message);
			}

			/// Checks if the result is greater than another number.
			greaterThan (result: any, greaterThanThisNumber: any, message: string = ""): void
			{
				if (result > greaterThanThisNumber)
					this.success (message);
				else
					this.error (message);
			}

			/// Checks if the result is less than another number.
			lessThan (result: any, lessThanThisNumber: any, message: string = ""): void
			{
				if (result > lessThanThisNumber)
					this.success (message);
				else
					this.error (message);
			}

			/// Checks if the incoming object is a number.
			isNumber (obj: any, message: string = ""): void
			{
				if (typeof (obj) == "number")
					this.success (message);
				else
					this.error (message);
			}

			/// Checks if the incoming object is a string.
			isString (obj: any, message: string = ""): void
			{
				if (typeof (obj) == "string")
					this.success (message);
				else
					this.error (message);
			}

			/// Checks if the incoming object is an array.
			isArray (obj: any, message: string = ""): void
			{
				if (obj instanceof Array)
					this.success (message);
				else
					this.error (message);
			}

			/// Checks if the incoming object is an object.
			isObject (obj: any, message: string = ""): void
			{
				if (typeof (obj) == "object")
					this.success (message);
				else
					this.error (message);
			}
		}

		/// A test to execute.
		export class Test
		{
			/// The name.
			name: string;
			/// Whether or not the tests successfully passed.
			passed: boolean[];
			/// The detailed results of the test.
			results: string[];
			/// The path to the application.
			applicationPath: string;
			/// The actual test to execute.
			func: Function;
			/// The amount of time in milliseconds before a test times out.
			timeout: number;

			constructor (name: string, applicationPath: string = "")
			{
				this.name = name;
				this.passed = [];
				this.results = [];
				this.applicationPath = applicationPath;
				this.func = null;
				this.timeout = 1000 * 60;
			}

			/// Execute the test. Store the result in this test, and return the test.
			execute (): Promise<Test>
			{
				let promise: Promise<Test> = new Promise<Test> (RadJav.keepContext(function (resolve, reject)
					{
						var that = this;

						let timeoutHandler = function(test: Test) {
							that.passed.push(false);
							that.results.push("Test timed out.");
							resolve(that);
						}

						let ret = that.func();

						if (ret instanceof Promise)
						{
							setTimeout(timeoutHandler, this.timeout);

							let result = RadJav.keepContext( function() {
								resolve (that);
							}, that);

							ret.then(result, result).catch(result);
						}
						else
						{
							resolve(that);
						}
					}, this));

				return (promise);
			}
		}

		/// Create a new test case.
		export class FunctionalTests
		{
			/// The path to the application.
			applicationPath: string;
			/// The tests to execute.
			tests: Test[];

			constructor (applicationPath: string)
			{
				this.applicationPath = applicationPath;
				this.tests = [];
			}

			/// Add a test to execute.
			addTest (test: Test | Function): void
			{
				if (typeof (test) == "function")
				{
					let tempFunc: Function = test;
					test = new Test (this.applicationPath, this.applicationPath);
					test.func = tempFunc;
				}

				this.tests.push (test);
			}

			/// Execute the tests.
			execute (): Promise<Test[]>
			{
				let promise: Promise<Test[]> = new Promise<Test[]> (RadJav.keepContext(function (resolve, reject)
					{
						if (this.tests.length == 0)
						{
							RadJav.Console.log("Nothing to execute, no tests found");
							resolve(this.tests);
							return;
						}

						if (RadJav.isDesktop())
						{
							//RadJav.Console.log("Args: "+RadJav.OS.args);

							let params = {isMaster: true,
											testCaseName: "",
											appPath: "",
											execFile: ""};
							
							if (RadJav.OS.args.length > 2 &&
								RadJav.OS.args[0] === "--slave")
							{
								params.isMaster = false;
								if (RadJav.OS.args.length > 2)
								{
									params.testCaseName = RadJav.OS.args[2];
								}
							}
							
							params.appPath = RadJav.OS.getApplicationPath();
							params.execFile = RadJav.OS.executingFile;

							if (params.isMaster)
							{
								let reportFileName = params.execFile.split('\\').pop().split('/').pop();
								reportFileName = reportFileName.split(".")[0]+".csv";

								var reporter = new CsvReporter(reportFileName);

								let currentTestIndex = 0;

								//RadJav.Console.log("Acting like a Master");

								var server = new RadJav.Net.WebServer();

								server.on("upgrade", RadJav.keepContext(function (connection, request)
								{
									var webSocket = RadJav.Net.handleUpgrade(connection, request);

									webSocket.on("message", RadJav.keepContext( function(data)
									{
										//RadJav.Console.log("WebSocket message received: "+data);
										
										let response = {status: "OK"};
										
										let testResult = new RadJav.Testing.Test(this.tests[currentTestIndex], "");
										testResult.results.push("Start test");
										testResult.passed.push(false);

										try
										{
											testResult = JSON.parse(data);
										}
										catch(err)
										{
											response = {status:"FAIL"};
										}
										finally
										{
											reporter.appendResult(testResult);
											webSocket.send(JSON.stringify(response));
										}

										currentTestIndex++;
										if (currentTestIndex == this.tests.length)
										{
											webSocket.close();
											resolve (this.tests);
											return;
										}

										let test = this.tests[currentTestIndex];

										let command = params.appPath+" "+params.execFile+" --slave "+"--testcase "+test.name;
										//RadJav.Console.log("Starting application: " + command);

										RadJav.OS.exec(command);

									}, this));
								}, this));

								server.on("close", function(){
									//RadJav.Console.log("Server closed");
								});

								server.on("error", function(err, description) {
									RadJav.Console.log("Server error: "+err+", "+description);
									resolve(this.tests);
								});

								server.start("127.0.0.1", 9898);

								let test = this.tests[currentTestIndex];

								let command = params.appPath+" "+params.execFile+" --slave "+"--testcase "+test.name;
								//RadJav.Console.log("Starting application: " + command);

								RadJav.OS.exec(command);
							}
							else
							{
								//RadJav.Console.log("Acting like a Slave");
								
								let test = null;

								for (let i = 0; i < this.tests.length; i++)
								{
									if (params.testCaseName === this.tests[i].name)
									{
										test = this.tests[i];
										break;
									}
								}

								var client = new RadJav.Net.WebSocketConnection();

								client.on("open", RadJav.keepContext(function ()
								{
									if (test != null)
									{
										test.execute().then(function (testObj) {
											let message = JSON.stringify(testObj);
											client.send(message);
										});
									}
									else
									{
										let message = {name: params.testCaseName, passed: [false]};
										client.send(JSON.stringify(message));
									}
								}, this));

								client.on("message", RadJav.keepContext(function (data)
								{
									let msg = JSON.parse(data);
									if(msg.status != null && msg.status == "OK")
									{
										//RadJav.Console.log("Got response: "+msg.status);
									}

									client.close();
									resolve(this.tests);
								}, this));

								client.on("error", RadJav.keepContext( function (err, description)
								{
									RadJav.Console.log("Unable to connect: "+err+", "+description);
									resolve(this.tests);
								}, this));

								client.on("close", RadJav.keepContext(function () {
									//RadJav.Console.log("Socket closed");
									resolve(this.tests);
								}, this));

								client.connect("ws://127.0.0.1:9898/testing");
							}
						}
						else if (RadJav.OS.type == "html5")
						{
							//TODO: add implementation
						}

						// If on desktop/mobile, create a WebSocket server here, inside the promise.
						// If in HTML5, open a new tab and create a connection back to the parent window.
						/*
						let promises: Promise<Test>[] = [];

						for (let iIdx = 0; iIdx < this.tests.length; iIdx++)
						{
							let test: Test = this.tests[iIdx];

							promises.push (test.execute ());
						}

						return (Promise.all (promises).then (function (tests)
							{
								resolve (tests);
							}));
						*/
					}, this));

				return (promise);
			}
		}

		export class CsvReporter
		{
			private _filePath: string;
			private _textFile: RadJav.IO.TextFile;
			private _delimiter: string;

			constructor (fileName: string, delimiter: string = ";")
			{
				this._filePath = RadJav.OS.getCurrentWorkingPath()+"/"+fileName;
				this._textFile = new RadJav.IO.TextFile();
				this._delimiter = delimiter;

				if(RadJav.IO.exists(this._filePath))
					RadJav.IO.deleteFile(this._filePath);
				
				let headerData = `Test name${this._delimiter}Steps${this._delimiter}Result\n`;
				this._textFile.writeFile(this._filePath, headerData, RadJav.IO.TextFile.write);
			}

			appendResult(test: Test): void
			{
				let testHeader = test.name+`${this._delimiter}${this._delimiter}\n`;
				this._textFile.writeFile(this._filePath, testHeader, RadJav.IO.TextFile.append);

				for(let i = 0; i < test.passed.length; i++)
				{
					let result = test.passed[i] ? "PASS" : "FAIL";
					let resultLine = `${this._delimiter}"${test.results[i]}"${this._delimiter}${result}\n`;
					this._textFile.writeFile(this._filePath, resultLine, RadJav.IO.TextFile.append);
				}
			}
		}

		/** Simulates keyboard input.
		* Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class KeyboardSimulator
		{
			/// Press a key.
			static keyPress (key: string): void
			{
			}

			/// Enter text.
			/// @param text The text to enter.
			/// @param delayBetweenKeyPresses The delay between key presses in milliseconds.
			static enterText (text: string, delayBetweenKeyPresses: number = 0): Promise<void>
			{
				let promise: Promise<void> = new Promise<void> (function (resolve, reject)
					{
						if (delayBetweenKeyPresses == 0)
						{
							for(let i = 0; i < text.length; i++)
							{
								RadJav.Testing.KeyboardSimulator.keyPress(text.charAt(i));
							}

							resolve ();
						}
						else
						{
							for(let i = 0; i <= text.length; i++)
							{
								if (i == text.length)
								{
									setTimeout (function (char)
										{
											resolve ();
										}, delayBetweenKeyPresses * i);

									break;
								}

								setTimeout (RadJav.keepContext (function (char)
									{
										RadJav.Testing.KeyboardSimulator.keyPress(char[0]);
									}, this, [text.charAt(i)]), delayBetweenKeyPresses * i);
							}
						}
					});

				return (promise);
			}
		}

		/** Simulates mouse input.
		* Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class MouseSimulator
		{
			/// Click a mouse button.
			static click (button: number): void
			{
			}

			/// Click a mouse button.
			static setPosition (pos: Vector2): void
			{
			}

			/// Move to a position and click.
			static moveClick (pos: Vector2, button: number = 0): void
			{
				RadJav.Testing.MouseSimulator.setPosition(pos);
				RadJav.Testing.MouseSimulator.click(button);
			}
		}

		/** Simulates touch input.
		* Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class TouchSimulator
		{
			/// Click a mouse button.
			static setPosition (pos: Vector2): void
			{
			}
		}

		/** Simulates accelerometer input.
		* Available on platforms: iOS,Android
		*/
		export class AccelerometerSimulator
		{
			/// Do a shake.
			static shake (pos: Vector2): void
			{
			}
		}
	}
}
