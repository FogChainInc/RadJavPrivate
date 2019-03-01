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

namespace RadJav
{
	/** @class RadJav.Testing
	* The testing class.
	* Available on platforms: Windows,Linux,OSX,iOS,Android
	*/
	export namespace Testing
	{
		/// The testing API.
		export class TestingAPI
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
					this.success ();
				else
					this.error (errorMessage);
			}

			/// Checks if the objects are equal.
			equal (expected: any, result: any, message: string = ""): void
			{
				if (expected == result)
					this.success (message);
				else
					this.error ();
			}

			/// Checks if the objects are not equal.
			notEqual (expected: any, result: any, message: string = ""): void
			{
				if (expected != result)
					this.success (message);
				else
					this.error ();
			}

			/// Checks if the result is greater than another number.
			greaterThan (result: any, greaterThanThisNumber: any, message: string = ""): void
			{
				if (result > greaterThanThisNumber)
					this.success (message);
				else
					this.error ();
			}

			/// Checks if the result is less than another number.
			lessThan (result: any, lessThanThisNumber: any, message: string = ""): void
			{
				if (result > lessThanThisNumber)
					this.success (message);
				else
					this.error ();
			}

			/// Checks if the incoming object is a number.
			isNumber (obj: any, message: string = ""): void
			{
				if (typeof (obj) == "number")
					this.success (message);
				else
					this.error ();
			}

			/// Checks if the incoming object is a string.
			isString (obj: any, message: string = ""): void
			{
				if (typeof (obj) == "string")
					this.success (message);
				else
					this.error ();
			}

			/// Checks if the incoming object is an array.
			isArray (obj: any, message: string = ""): void
			{
				if (obj instanceof Array)
					this.success (message);
				else
					this.error ();
			}

			/// Checks if the incoming object is an object.
			isObject (obj: any, message: string = ""): void
			{
				if (typeof (obj) == "object")
					this.success (message);
				else
					this.error ();
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

			constructor (name: string, applicationPath: string)
			{
				this.name = name;
				this.passed = [];
				this.results = [];
				this.applicationPath = applicationPath;
				this.func = null;
			}

			/// Execute the test. Store the result in this test, and return the test.
			execute (): Promise<Test>
			{
				let promise: Promise<Test> = new Promise<Test> (function (resolve, reject)
					{
						/// If on desktop/mobile open RadJavVM and have it open the file stored at applicationPath.
						/// Then connect to the WebSocket server and have it transfer the results of the test.
						/// If on HTML5, simply return the results of the test back to the parent window.
						this.func ();

						resolve (this);
					});

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
				let promise: Promise<Test[]> = new Promise<Test[]> (function (resolve, reject)
					{
						// If on desktop/mobile, create a WebSocket server here, inside the promise.
						// If in HTML5, open a new tab and create a connection back to the parent window.

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
					});

				return (promise);
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
