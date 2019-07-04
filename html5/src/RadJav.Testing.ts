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
	/** 
	* The testing class.
	* Available on platforms: Windows,Linux,OSX,iOS,Android
	*/
	export namespace Testing
	{
		/// Tests properties inside classes.
		export interface ClassPropertyTester
		{
			/// Name of the property. If this is an array, this can test multiple
			/// properties using the same test function below. Be aware, every 
			/// name index in this array must match up with every type index.
			name: string | string[];
			/// Type of property. If this is an array, every index must match up 
			/// with every name index.
			type: string | string[];
			/// The test function to execute to test this property.
			test?: (tlib: TestLibrary) => boolean | { assertion: boolean, message: string };
		}

		/// Tests classes.
		export interface ClassTester
		{
			/// Check if the name of the class matches the class.
			name: string;
			/// The function to test this class.
			test?: (tlib: TestLibrary) => boolean | { assertion: boolean, message: string };
			/// The properties in this class to test.
			properties?: ClassPropertyTester[];
		}

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

			/// Test a class.
			testClass (classToTest: any, classTester: ClassTester)
			{
				if (classToTest.name !== classTester.name)
				{
					this.error ("Class name does not match!");

					return;
				}

				let successMsg: string = classToTest.name + " passed testing.";

				if (classTester.test !== null)
				{
					let result: boolean | { assertion: boolean, message: string } = classTester.test (this);

					if (result !== null)
					{
						if (typeof (result) === "boolean")
						{
							if ((<boolean>result) === false)
							{
								this.error (classToTest.name + " failed testing.");

								return;
							}
						}
						else
						{
							let resultRes: { assertion: boolean, message: string } = (<{ assertion: boolean, message: string }>result);

							if (resultRes.assertion === false)
							{
								this.error (resultRes.message);

								return;
							}
							else
								successMsg = resultRes.message;
						}
					}
				}

				for (let iIdx = 0; iIdx < classTester.properties.length; iIdx++)
				{
					let prop: ClassPropertyTester = classTester.properties[iIdx];

					if (typeof (prop) === "string")
					{
						let propertyName: string = (<string>prop["name"]);
						let propertyType: string = (<string>prop["type"]);

						if (classToTest[propertyName] === null)
						{
							this.error ("Property " + propertyName + " is missing!");

							return;
						}

						if (propertyType !== null)
						{
							let foundType = false;
							let globalVar = Function ("return (this);")();

							if (typeof (classToTest[propertyName]) === propertyType)
								foundType = true;

							if (! (classToTest[propertyName] instanceof globalVar["propertyType"]))
								foundType = true;

							if (foundType === true)
							{
								this.error ("Property " + propertyName + " is not of type " + prop["type"] + "!");

								return;
							}
						}
					}
					else
					{
						let propertyNames: string[] = (<string[]>prop.name);
						let propertyTypes: string[] = (<string[]>prop.type);

						for (let iJdx = 0; iIdx < propertyNames.length; iIdx++)
						{
							let propertyName: string = propertyNames[iIdx];
							let propertyType: string = propertyTypes[iIdx];
	
							if (classToTest[propertyName] === null)
							{
								this.error ("Property " + propertyName + " is missing!");

								return;
							}

							if (propertyType !== null)
							{
								let foundType = false;
								let globalVar = Function ("return (this);")();
	
								if (typeof (classToTest[propertyName]) === propertyType)
									foundType = true;
	
								if (! (classToTest[propertyName] instanceof globalVar["propertyType"]))
									foundType = true;
	
								if (foundType === true)
								{
									this.error ("Property " + propertyName + " is not of type " + prop["type"] + "!");

									return;
								}
							}
						}
					}

					if (prop.test !== null)
					{
						let result: boolean | { assertion: boolean, message: string } = prop.test (this);
	
						if (result !== null)
						{
							if (typeof (result) === "boolean")
							{
								if ((<boolean>result) === false)
								{
									this.error (JSON.stringify (prop.name) + " failed testing.");
	
									return;
								}
							}
							else
							{
								let resultRes: { assertion: boolean, message: string } = (<{ assertion: boolean, message: string }>result);
	
								if (resultRes.assertion === false)
								{
									this.error (resultRes.message);
	
									return;
								}
							}
						}
					}
				}

				this.success (successMsg);
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

						let timeoutHandler = function() {
							that.passed.push(false);
							that.results.push("Test timed out.");
							resolve(that);
						}

						let ret = that.func();

						if (ret instanceof Promise)
						{
							var timeoutId = setTimeout(timeoutHandler, this.timeout);

							let result = RadJav.keepContext( function() {
								clearTimeout(timeoutId);
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
			/// Identify if we are act like a node which execute real test case
			node: boolean;
			/// The listening address.
			listenAddress: string;
			/// The port.
			port: number;
			/// The server
			server: RadJav.Net.WebServer;
			/// The client
			client: RadJav.Net.WebSocketConnection;

			constructor (applicationPath: string)
			{
				this.applicationPath = applicationPath;
				this.tests = [];
				this.node = false;
				this.listenAddress = "127.0.0.1";
				this.port = 9898;
				this.server = null;
				this.client = null;

			}

			/// Are we child process?
			isNode (): boolean
			{
				return this.node;
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

			/// Add an assert test.
			assertTest (testName: string, expression: boolean, errorMessage: string = "")
			{
				let test: Test = new Test (this.applicationPath, this.applicationPath);

				test.func = (tlib: TestLibrary) =>
					{
						tlib.assert (expression, errorMessage);
					};

				this.addTest (test);
			}

			/// Execute the tests.
			execute (): Promise<Test[]>
			{
				let promise: Promise<Test[]> = new Promise<Test[]> (RadJav.keepContext(function (resolve, reject)
					{
						if (this.tests.length == 0)
						{
							//RadJav.Console.log("Nothing to execute, no tests found");
							resolve(this.tests);
							return;
						}

						if (RadJav.isDesktop())
						{
							//RadJav.Console.log("Args: "+RadJav.OS.args);

							let params = { isNode: false,
											testCaseName: "",
											appPath: "",
											execFile: "" };
							let nodeArg: number = RadJav.OS.searchArgs ("--node");

							if (nodeArg > -1)
							{
								params.isNode = true;

								let testnameArg: number = RadJav.OS.searchArgs("--testcase");
								if (testnameArg > -1)
								{
									params.testCaseName = RadJav.OS.args[(testnameArg + 1)];
								}
							}

							params.appPath = RadJav.OS.getApplicationPath();
							params.execFile = RadJav.OS.executingFile;

							if (!params.isNode)
							{
								// Acting like the server
								let reportFileName = params.execFile.split('\\').pop().split('/').pop();

								if (reportFileName.indexOf (".") > -1)
									reportFileName = reportFileName.split(".")[0];

								reportFileName = reportFileName + ".csv";

								var reporter = new CsvReporter(reportFileName);
								var currentTestIndex = -1;
								var testExecutionTimer = null;
								var nodeConnection = null;

								var reportTestResult = RadJav.keepContext(function() {
									let test = this.tests[currentTestIndex];
									if ( test != undefined)
									{
										if (test.passed.length == 0)
										{
											test = new RadJav.Testing.Test(this.tests[currentTestIndex].name, "");
											test.results.push("Start test");
											test.passed.push(false);
										}
										
										reporter.appendResult(test);
									}
								}, this);

								var runNextTest = RadJav.keepContext( function()
								{
									//RadJav.Console.log("Incrementing test case");
									currentTestIndex++;
									if (currentTestIndex == this.tests.length)
									{
										//RadJav.Console.log("Last test case executed");
										resolve (this.tests);
										return;
									}

									//RadJav.Console.log("Getting test case");
									let test: Test = this.tests[currentTestIndex];

									//RadJav.Console.log("Executing test case "+test.name);
									var process: RadJav.OS.SystemProcess = new RadJav.OS.SystemProcess (
										params.appPath, [params.execFile, "--node", "--testcase", test.name]);
									
									process.on("exit", RadJav.keepContext(function(code) {
										clearTimeout(testExecutionTimer);
										reportTestResult();
										runNextTest();
									}, this));

									process.on("error", RadJav.keepContext(function(err, description) {
										clearTimeout(testExecutionTimer);
										reportTestResult();
										runNextTest();
									}, this));

									process.execute ();

									testExecutionTimer = setTimeout( RadJav.keepContext(function () {
										//RadJav.Console.log("Test timed out");
										let test = this.tests[currentTestIndex];
										test.results.push("Test reach it's timeout");
										test.passed.push(false);

										// Kill child
										process.kill();
									}, this), test.timeout);
								}, this);

								this.server = new RadJav.Net.WebServer();

								this.server.on("upgrade", RadJav.keepContext(function (connection, request)
								{
									//RadJav.Console.log("WebSocket new connection");
									nodeConnection = RadJav.Net.handleUpgrade(connection, request);

									nodeConnection.on("message", RadJav.keepContext( function(data)
									{
										//RadJav.Console.log("WebSocket message received: "+data);
										
										let response = {status: "OK"};
										
										try
										{
											//RadJav.Console.log("Parsing message");
											let testResult = JSON.parse(data);

											if (testResult.name === this.tests[currentTestIndex].name)
											{
												this.tests[currentTestIndex].passed = testResult["passed"];
												this.tests[currentTestIndex].results = testResult["results"];
											}
										}
										catch(err)
										{
											//RadJav.Console.log("Unable to update test result");
											response = {status:"FAIL"};
										}
										finally
										{
											//RadJav.Console.log("Sending response "+JSON.stringify(response));
											nodeConnection.send(JSON.stringify(response));
										}
									}, this));
								}, this));

								this.server.on("error", function(err, description) {
									//RadJav.Console.log("Server error: "+err+", "+description);
									resolve(this.tests);
								});

								this.server.start(this.listenAddress, this.port);

								runNextTest();
							}
							else
							{
								// Acting like a node
								
								let test = null;

								for (let i = 0; i < this.tests.length; i++)
								{
									if (params.testCaseName === this.tests[i].name)
									{
										test = this.tests[i];
										break;
									}
								}

								this.client = new RadJav.Net.WebSocketConnection();

								this.client.on("open", RadJav.keepContext(function ()
								{
									if (test != null)
									{
										test.execute().then(RadJav.keepContext(function (testObj) {
											let message = JSON.stringify(testObj);
											this.client.send(message);
										}, this));
									}
									else
									{
										let message = new RadJav.Testing.Test(params.testCaseName);
										message.passed.push(false);
										message.results.push("Test case not found");
										this.client.send(JSON.stringify(message));
									}
								}, this));

								this.client.on("message", RadJav.keepContext(function (data)
								{
									let msg = JSON.parse(data);
									if(msg.status != null && msg.status == "OK")
									{
										//RadJav.Console.log("Got response: "+msg.status);
									}

									this.client.close();
									resolve(this.tests);
								}, this));

								this.client.on("error", RadJav.keepContext( function (err, description)
								{
									//RadJav.Console.log("WebSocket error: "+err+", "+description);
									resolve(this.tests);
								}, this));

								this.client.on("close", RadJav.keepContext(function () {
									//RadJav.Console.log("WebSocket closed");
									resolve(this.tests);
								}, this));

								this.client.connect("ws://" + this.listenAddress + ":" + this.port + "/testing");
							}
						}
						else if (RadJav.OS.type == "html5")
						{
							let testList = this.tests.reverse();

							var executeSequentially = function (tests : Test[])
							{
								if (tests && tests.length > 0) {
									var test = tests.shift();
								
									return test.execute().then(function(output: Test) {
										return executeSequentially(tests).then(function(outputs: Test[]) {
											outputs.push(output);
								
											return Promise.resolve(outputs);  
										  });
									});
								}
								
								return Promise.resolve([]);
							}
	
							executeSequentially(testList).then( RadJav.keepContext( function(testResults: Test[]) {
								this.tests = testResults;
								let reportFileName = RadJav.OS.executingFile.split('\\').pop().split('/').pop();
	
								if (reportFileName.indexOf (".") > -1)
									reportFileName = reportFileName.split(".")[0];
		
								reportFileName = reportFileName + ".csv";
		
								var reporter = new CsvReporter(reportFileName);
								for (let i = 0; i < this.tests.length; i++)
								{
									reporter.appendResult(this.tests[i]);
								}
								
								resolve(this.tests);
							}, this));
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
			private _delimiter: string;

			constructor (fileName: string, delimiter: string = ";")
			{
				this._filePath = RadJav.OS.getCurrentWorkingPath()+"/"+fileName;
				this._delimiter = delimiter;

				if(RadJav.IO.exists(this._filePath))
					RadJav.IO.deleteFile(this._filePath);
				
				let headerData = `Test name${this._delimiter}Steps${this._delimiter}Result\n`;
				RadJav.IO.TextFile.writeToFile(this._filePath, headerData, RadJav.IO.AccessType.write);
			}

			appendResult(test: Test): void
			{
				let testHeader = test.name+`${this._delimiter}${this._delimiter}\n`;
				RadJav.IO.TextFile.writeToFile(this._filePath, testHeader, RadJav.IO.AccessType.append);

				for(let i = 0; i < test.passed.length; i++)
				{
					let result = test.passed[i] ? "PASS" : "FAIL";
					let resultLine = `${this._delimiter}"${test.results[i]}"${this._delimiter}${result}\n`;
					RadJav.IO.TextFile.writeToFile(this._filePath, resultLine, RadJav.IO.AccessType.append);
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

		/** 
		 * Simulates mouse input.
		 * Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class MouseSimulator
		{
			/// Click a mouse button.
			static click (button: number = 0): void
			{
			}

			/// Click a mouse button.
			static setPosition (pos: RadJav.Vector2): void
			{
			}

			/// Double click a mouse button.
			static doubleClick(button: number = 0): void
			{
			}

			/// Move to a position and click.
			static moveClick (pos: RadJav.Vector2 | RadJav.GUI.GObject, button: number = 0): void
			{
				if (pos instanceof RadJav.Vector2)
					RadJav.Testing.MouseSimulator.setPosition(pos);

				if (pos instanceof RadJav.GUI.GObject)
					RadJav.Testing.MouseSimulator.setPosition((<RadJav.GUI.GObject>pos).getPosition ());

				RadJav.Testing.MouseSimulator.click(button);
			}

			/// Move to a position and double click.
			static moveDoubleClick (pos: RadJav.Vector2 | RadJav.GUI.GObject, button: number = 0): void
			{
				if (pos instanceof RadJav.Vector2)
					RadJav.Testing.MouseSimulator.setPosition(pos);

				if (pos instanceof RadJav.GUI.GObject)
					RadJav.Testing.MouseSimulator.setPosition((<RadJav.GUI.GObject>pos).getPosition ());

				RadJav.Testing.MouseSimulator.doubleClick(button);
			}

			/// Wheel a mouse button.
			static wheel (vertical: number = 0, horizontal: number = 0): void
			{
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
