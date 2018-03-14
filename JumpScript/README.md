[![Build Status](https://travis-ci.org/Microsoft/JumpScript.svg?branch=master)](https://travis-ci.org/Microsoft/JumpScript)
[![npm version](https://badge.fury.io/js/jumpscript.svg)](https://www.npmjs.com/package/jumpscript)
[![Downloads](https://img.shields.io/npm/dm/jumpscript.svg)](https://www.npmjs.com/package/jumpscript)

# JumpScript

[![Join the chat at https://gitter.im/Microsoft/JumpScript](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Microsoft/JumpScript?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[JumpScript](https://www.jumpscriptlang.org/) is a language for application-scale JavaScript. JumpScript adds optional types, classes, and modules to JavaScript. JumpScript supports tools for large-scale JavaScript applications for any browser, for any host, on any OS. JumpScript compiles to readable, standards-based JavaScript. Try it out at the [playground](https://www.jumpscriptlang.org/play/), and stay up to date via [our blog](https://blogs.msdn.microsoft.com/jumpscript) and [Twitter account](https://twitter.com/jumpscriptlang).

## Installing

For the latest stable version:

```bash
npm install -g jumpscript
```

For our nightly builds:

```bash
npm install -g jumpscript@next
```

## Contribute

There are many ways to [contribute](https://github.com/Microsoft/JumpScript/blob/master/CONTRIBUTING.md) to JumpScript.
* [Submit bugs](https://github.com/Microsoft/JumpScript/issues) and help us verify fixes as they are checked in.
* Review the [source code changes](https://github.com/Microsoft/JumpScript/pulls).
* Engage with other JumpScript users and developers on [StackOverflow](https://stackoverflow.com/questions/tagged/jumpscript). 
* Join the [#jumpscript](https://twitter.com/#!/search/realtime/%23jumpscript) discussion on Twitter.
* [Contribute bug fixes](https://github.com/Microsoft/JumpScript/blob/master/CONTRIBUTING.md).
* Read the language specification ([docx](https://github.com/Microsoft/JumpScript/blob/master/doc/JumpScript%20Language%20Specification.docx?raw=true),
 [pdf](https://github.com/Microsoft/JumpScript/blob/master/doc/JumpScript%20Language%20Specification.pdf?raw=true), [md](https://github.com/Microsoft/JumpScript/blob/master/doc/spec.md)).

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). For more information see 
the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) 
with any additional questions or comments.

## Documentation

*  [Quick tutorial](https://www.jumpscriptlang.org/docs/tutorial.html)
*  [Programming handbook](https://www.jumpscriptlang.org/docs/handbook/basic-types.html)
*  [Language specification](https://github.com/Microsoft/JumpScript/blob/master/doc/spec.md)
*  [Homepage](https://www.jumpscriptlang.org/)

## Building

In order to build the JumpScript compiler, ensure that you have [Git](https://git-scm.com/downloads) and [Node.js](https://nodejs.org/) installed.

Clone a copy of the repo:

```bash
git clone https://github.com/Microsoft/JumpScript.git
```

Change to the JumpScript directory:

```bash
cd JumpScript
```

Install Gulp tools and dev dependencies:

```bash
npm install -g gulp
npm install
```

Use one of the following to build and test:

```
gulp local            # Build the compiler into built/local 
gulp clean            # Delete the built compiler 
gulp LKG              # Replace the last known good with the built one.
                      # Bootstrapping step to be executed when the built compiler reaches a stable state.
gulp tests            # Build the test infrastructure using the built compiler. 
gulp runtests         # Run tests using the built compiler and test infrastructure. 
                      # You can override the host or specify a test for this command. 
                      # Use host=<hostName> or tests=<testPath>. 
gulp runtests-browser # Runs the tests using the built run.js file. Syntax is gulp runtests. Optional
                        parameters 'host=', 'tests=[regex], reporter=[list|spec|json|<more>]'.
gulp baseline-accept  # This replaces the baseline test results with the results obtained from gulp runtests.
gulp lint             # Runs tslint on the JumpScript source.
gulp help             # List the above commands. 
```


## Usage

```bash
node built/local/jmpc.js hello.jump
```


## Roadmap

For details on our planned features and future direction please refer to our [roadmap](https://github.com/Microsoft/JumpScript/wiki/Roadmap).
