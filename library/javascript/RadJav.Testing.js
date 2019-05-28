var RadJav;
(function (RadJav) {
    var Testing;
    (function (Testing) {
        var TestLibrary = (function () {
            function TestLibrary(test) {
                this.test = test;
            }
            TestLibrary.prototype.success = function (message) {
                if (message === void 0) { message = ""; }
                this.test.passed.push(true);
                this.test.results.push(message);
            };
            TestLibrary.prototype.error = function (message, expected, result) {
                if (message === void 0) { message = ""; }
                if (expected === void 0) { expected = undefined; }
                if (result === void 0) { result = undefined; }
                this.test.passed.push(false);
                this.test.results.push(message);
            };
            TestLibrary.prototype.assert = function (expression, errorMessage) {
                if (errorMessage === void 0) { errorMessage = ""; }
                if (expression == true)
                    this.success(errorMessage);
                else
                    this.error(errorMessage);
            };
            TestLibrary.prototype.equal = function (expected, result, message) {
                if (message === void 0) { message = ""; }
                if (expected == result)
                    this.success(message);
                else
                    this.error(message);
            };
            TestLibrary.prototype.notEqual = function (expected, result, message) {
                if (message === void 0) { message = ""; }
                if (expected != result)
                    this.success(message);
                else
                    this.error(message);
            };
            TestLibrary.prototype.greaterThan = function (result, greaterThanThisNumber, message) {
                if (message === void 0) { message = ""; }
                if (result > greaterThanThisNumber)
                    this.success(message);
                else
                    this.error(message);
            };
            TestLibrary.prototype.lessThan = function (result, lessThanThisNumber, message) {
                if (message === void 0) { message = ""; }
                if (result > lessThanThisNumber)
                    this.success(message);
                else
                    this.error(message);
            };
            TestLibrary.prototype.isNumber = function (obj, message) {
                if (message === void 0) { message = ""; }
                if (typeof (obj) == "number")
                    this.success(message);
                else
                    this.error(message);
            };
            TestLibrary.prototype.isString = function (obj, message) {
                if (message === void 0) { message = ""; }
                if (typeof (obj) == "string")
                    this.success(message);
                else
                    this.error(message);
            };
            TestLibrary.prototype.isArray = function (obj, message) {
                if (message === void 0) { message = ""; }
                if (obj instanceof Array)
                    this.success(message);
                else
                    this.error(message);
            };
            TestLibrary.prototype.isObject = function (obj, message) {
                if (message === void 0) { message = ""; }
                if (typeof (obj) == "object")
                    this.success(message);
                else
                    this.error(message);
            };
            return TestLibrary;
        }());
        Testing.TestLibrary = TestLibrary;
        var Test = (function () {
            function Test(name, applicationPath) {
                if (applicationPath === void 0) { applicationPath = ""; }
                this.name = name;
                this.passed = [];
                this.results = [];
                this.applicationPath = applicationPath;
                this.func = null;
                this.timeout = 1000 * 60;
            }
            Test.prototype.execute = function () {
                var promise = new Promise(RadJav.keepContext(function (resolve, reject) {
                    var that = this;
                    var timeoutHandler = function () {
                        that.passed.push(false);
                        that.results.push("Test timed out.");
                        resolve(that);
                    };
                    var ret = that.func();
                    if (ret instanceof Promise) {
                        setTimeout(timeoutHandler, this.timeout);
                        var result = RadJav.keepContext(function () {
                            resolve(that);
                        }, that);
                        ret.then(result, result)["catch"](result);
                    }
                    else {
                        resolve(that);
                    }
                }, this));
                return (promise);
            };
            return Test;
        }());
        Testing.Test = Test;
        var FunctionalTests = (function () {
            function FunctionalTests(applicationPath) {
                this.applicationPath = applicationPath;
                this.tests = [];
                this.node = false;
                this.listenAddress = "127.0.0.1";
                this.port = 9898;
            }
            FunctionalTests.prototype.isNode = function () {
                return this.node;
            };
            FunctionalTests.prototype.addTest = function (test) {
                if (typeof (test) == "function") {
                    var tempFunc = test;
                    test = new Test(this.applicationPath, this.applicationPath);
                    test.func = tempFunc;
                }
                this.tests.push(test);
            };
            FunctionalTests.prototype.execute = function () {
                var promise = new Promise(RadJav.keepContext(function (resolve, reject) {
                    if (this.tests.length == 0) {
                        RadJav.Console.log("Nothing to execute, no tests found");
                        resolve(this.tests);
                        return;
                    }
                    if (RadJav.isDesktop()) {
                        var params_1 = { isNode: false,
                            testCaseName: "",
                            appPath: "",
                            execFile: "" };
                        var nodeArg = RadJav.OS.searchArgs("--node");
                        if (nodeArg > -1) {
                            params_1.isNode = true;
                            var testnameArg = RadJav.OS.searchArgs("--testcase");
                            if (testnameArg > -1) {
                                params_1.testCaseName = RadJav.OS.args[(testnameArg + 1)];
                            }
                        }
                        params_1.appPath = RadJav.OS.getApplicationPath();
                        params_1.execFile = RadJav.OS.executingFile;
                        if (!params_1.isNode) {
                            var reportFileName = params_1.execFile.split('\\').pop().split('/').pop();
                            if (reportFileName.indexOf(".") > -1)
                                reportFileName = reportFileName.split(".")[0];
                            reportFileName = reportFileName + ".csv";
                            var reporter = new CsvReporter(reportFileName);
                            var currentTestIndex_1 = 0;
                            var server = new RadJav.Net.WebServer();
                            server.on("upgrade", RadJav.keepContext(function (connection, request) {
                                var webSocket = RadJav.Net.handleUpgrade(connection, request);
                                webSocket.on("message", RadJav.keepContext(function (data) {
                                    var response = { status: "OK" };
                                    var testResult = new RadJav.Testing.Test(this.tests[currentTestIndex_1], "");
                                    testResult.results.push("Start test");
                                    testResult.passed.push(false);
                                    try {
                                        testResult = JSON.parse(data);
                                    }
                                    catch (err) {
                                        response = { status: "FAIL" };
                                    }
                                    finally {
                                        reporter.appendResult(testResult);
                                        webSocket.send(JSON.stringify(response));
                                    }
                                    currentTestIndex_1++;
                                    if (currentTestIndex_1 == this.tests.length) {
                                        webSocket.close();
                                        resolve(this.tests);
                                        return;
                                    }
                                    var test = this.tests[currentTestIndex_1];
                                    var process = new RadJav.OS.SystemProcess(params_1.appPath, [params_1.execFile, "--node", "--testcase", test.name]);
                                    process.execute();
                                }, this));
                            }, this));
                            server.on("close", function () {
                            });
                            server.on("error", function (err, description) {
                                RadJav.Console.log("Server error: " + err + ", " + description);
                                resolve(this.tests);
                            });
                            server.start(this.listenAddress, this.port);
                            var test = this.tests[currentTestIndex_1];
                            var process = new RadJav.OS.SystemProcess(params_1.appPath, [params_1.execFile, "--node", "--testcase", test.name]);
                            process.execute();
                        }
                        else {
                            var test_1 = null;
                            for (var i = 0; i < this.tests.length; i++) {
                                if (params_1.testCaseName === this.tests[i].name) {
                                    test_1 = this.tests[i];
                                    break;
                                }
                            }
                            var client = new RadJav.Net.WebSocketConnection();
                            client.on("open", RadJav.keepContext(function () {
                                if (test_1 != null) {
                                    test_1.execute().then(function (testObj) {
                                        var message = JSON.stringify(testObj);
                                        client.send(message);
                                    });
                                }
                                else {
                                    var message = new RadJav.Testing.Test(params_1.testCaseName);
                                    message.passed.push(false);
                                    message.results.push("Test case not found");
                                    client.send(JSON.stringify(message));
                                }
                            }, this));
                            client.on("message", RadJav.keepContext(function (data) {
                                var msg = JSON.parse(data);
                                if (msg.status != null && msg.status == "OK") {
                                }
                                client.close();
                                resolve(this.tests);
                            }, this));
                            client.on("error", RadJav.keepContext(function (err, description) {
                                RadJav.Console.log("Unable to connect: " + err + ", " + description);
                                resolve(this.tests);
                            }, this));
                            client.on("close", RadJav.keepContext(function () {
                                resolve(this.tests);
                            }, this));
                            client.connect("ws://" + this.listenAddress + ":" + this.port + "/testing");
                        }
                    }
                    else if (RadJav.OS.type == "html5") {
                        var testList = this.tests.reverse();
                        var executeSequentially = function (tests) {
                            if (tests && tests.length > 0) {
                                var test = tests.shift();
                                return test.execute().then(function (output) {
                                    return executeSequentially(tests).then(function (outputs) {
                                        outputs.push(output);
                                        return Promise.resolve(outputs);
                                    });
                                });
                            }
                            return Promise.resolve([]);
                        };
                        executeSequentially(testList).then(RadJav.keepContext(function (testResults) {
                            this.tests = testResults;
                            var reportFileName = RadJav.OS.executingFile.split('\\').pop().split('/').pop();
                            if (reportFileName.indexOf(".") > -1)
                                reportFileName = reportFileName.split(".")[0];
                            reportFileName = reportFileName + ".csv";
                            var reporter = new CsvReporter(reportFileName);
                            for (var i = 0; i < this.tests.length; i++) {
                                reporter.appendResult(this.tests[i]);
                            }
                            resolve(this.tests);
                        }, this));
                    }
                }, this));
                return (promise);
            };
            return FunctionalTests;
        }());
        Testing.FunctionalTests = FunctionalTests;
        var CsvReporter = (function () {
            function CsvReporter(fileName, delimiter) {
                if (delimiter === void 0) { delimiter = ";"; }
                this._filePath = RadJav.OS.getCurrentWorkingPath() + "/" + fileName;
                this._delimiter = delimiter;
                if (RadJav.IO.exists(this._filePath))
                    RadJav.IO.deleteFile(this._filePath);
                var headerData = "Test name" + this._delimiter + "Steps" + this._delimiter + "Result\n";
                RadJav.IO.TextFile.writeToFile(this._filePath, headerData, RadJav.IO.AccessType.write);
            }
            CsvReporter.prototype.appendResult = function (test) {
                var testHeader = test.name + ("" + this._delimiter + this._delimiter + "\n");
                RadJav.IO.TextFile.writeToFile(this._filePath, testHeader, RadJav.IO.AccessType.append);
                for (var i = 0; i < test.passed.length; i++) {
                    var result = test.passed[i] ? "PASS" : "FAIL";
                    var resultLine = this._delimiter + "\"" + test.results[i] + "\"" + this._delimiter + result + "\n";
                    RadJav.IO.TextFile.writeToFile(this._filePath, resultLine, RadJav.IO.AccessType.append);
                }
            };
            return CsvReporter;
        }());
        Testing.CsvReporter = CsvReporter;
        var KeyboardSimulator = (function () {
            function KeyboardSimulator() {
            }
            KeyboardSimulator.keyPress = function (key) {
            };
            KeyboardSimulator.enterText = function (text, delayBetweenKeyPresses) {
                if (delayBetweenKeyPresses === void 0) { delayBetweenKeyPresses = 0; }
                var promise = new Promise(function (resolve, reject) {
                    if (delayBetweenKeyPresses == 0) {
                        for (var i = 0; i < text.length; i++) {
                            RadJav.Testing.KeyboardSimulator.keyPress(text.charAt(i));
                        }
                        resolve();
                    }
                    else {
                        for (var i = 0; i <= text.length; i++) {
                            if (i == text.length) {
                                setTimeout(function (char) {
                                    resolve();
                                }, delayBetweenKeyPresses * i);
                                break;
                            }
                            setTimeout(RadJav.keepContext(function (char) {
                                RadJav.Testing.KeyboardSimulator.keyPress(char[0]);
                            }, this, [text.charAt(i)]), delayBetweenKeyPresses * i);
                        }
                    }
                });
                return (promise);
            };
            return KeyboardSimulator;
        }());
        Testing.KeyboardSimulator = KeyboardSimulator;
        var MouseSimulator = (function () {
            function MouseSimulator() {
            }
            MouseSimulator.click = function (button) {
            };
            MouseSimulator.setPosition = function (pos) {
            };
            MouseSimulator.moveClick = function (pos, button) {
                if (button === void 0) { button = 0; }
                RadJav.Testing.MouseSimulator.setPosition(pos);
                RadJav.Testing.MouseSimulator.click(button);
            };
            return MouseSimulator;
        }());
        Testing.MouseSimulator = MouseSimulator;
        var TouchSimulator = (function () {
            function TouchSimulator() {
            }
            TouchSimulator.setPosition = function (pos) {
            };
            return TouchSimulator;
        }());
        Testing.TouchSimulator = TouchSimulator;
        var AccelerometerSimulator = (function () {
            function AccelerometerSimulator() {
            }
            AccelerometerSimulator.shake = function (pos) {
            };
            return AccelerometerSimulator;
        }());
        Testing.AccelerometerSimulator = AccelerometerSimulator;
    })(Testing = RadJav.Testing || (RadJav.Testing = {}));
})(RadJav || (RadJav = {}));
//# sourceMappingURL=RadJav.Testing.js.map