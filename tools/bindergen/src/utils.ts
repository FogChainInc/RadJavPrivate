/// Show an error.
export function showError (err: string): void
{
	console.error (err);
}

/// Keep a context.
export function keepContext(func: Function, context: object, val?: any): any
{
	let objReturn = function ()
		{
			let aryArgs = Array.prototype.slice.call(arguments);

			if (val != undefined)
				aryArgs.push(val);

			if (context == null)
				// @ts-ignore
				return func.apply(this, aryArgs);
			else
				return func.apply(context, aryArgs);
		};

	return objReturn;
}

/// Perform a deep copy of an object. This has been copied from jQuery.
/// Thank you jQuery!
export function clone(...obj:any[]): any[] {
	var options,
		name,
		src,
		copy,
		copyIsArray,
		clone,
		target = arguments[0] || {},
		i = 1,
		length = arguments.length,
		deep = false;

	// Handle a deep copy situation
	if (typeof target === "boolean") {
		deep = target;

		// Skip the boolean and the target
		target = arguments[i] || {};
		i++;
	}

	var isPlainObject = function(obj) {
		var proto, Ctor;

		// Detect obvious negatives
		// Use toString instead of jQuery.type to catch host objects
		if (!obj || obj.toString() !== "[object Object]") {
			return false;
		}

		proto = Object.getPrototypeOf(obj);

		// Objects with no prototype (e.g., `Object.create( null )`) are plain
		if (!proto) {
			return true;
		}

		// Objects with prototype are plain iff they were constructed by a global Object function
		Ctor = proto.hasOwnProperty("constructor") && proto.constructor;
		return (
			typeof Ctor === "function" &&
			Ctor.toString() === Ctor.toString.call(Object)
		);
	};

	var isFunction = function(obj) {
		return typeof obj === "function";
	};

	// Handle case when target is a string or something (possible in deep copy)
	if (typeof target !== "object" && !isFunction(target)) {
		target = {};
	}

	// Extend jQuery itself if only one argument is passed
	if (i === length) {
		// @ts-ignore
		target = this;
		i--;
	}

	for (; i < length; i++) {
		// Only deal with non-null/undefined values
		if ((options = arguments[i]) != null) {
			// Extend the base object
			for (name in options) {
				src = target[name];
				copy = options[name];

				// Prevent never-ending loop
				if (target === copy) {
					continue;
				}

				// Recurse if we're merging plain objects or arrays
				if (
					deep &&
					copy &&
					(isPlainObject(copy) || (copyIsArray = Array.isArray(copy)))
				) {
					if (copyIsArray) {
						copyIsArray = false;
						clone = src && Array.isArray(src) ? src : [];
					} else {
						clone = src && isPlainObject(src) ? src : {};
					}

					// Never move original objects, clone them
					target[name] = clone(deep, clone, copy);

					// Don't bring in undefined values
				} else if (copy !== undefined) {
					target[name] = copy;
				}
			}
		}
	}

	// Return the modified object
	return target;
}

/** Perform a deep copy of an object.
* Available on platforms: Windows,Linux,OSX,HTML5
* @param {Object} obj The object to clone.
* @return {Object} The cloned object.
*/
export function cloneObject(obj: any): any
{
	return clone({}, obj);
}

/** Perform a deep copy of an array.
 * Available on platforms: Windows,Linux,OSX,HTML5
 * @param {Array} obj The array to clone.
 * @return {Array} The cloned array.
 */
export function cloneArray(obj: any[]): any[] {
	return clone([], obj);
}

/// Copy missing properties from an object from src to dest.
export function copyMissingProperties (src: any, dest: any): void
{
	for (let srcKey in src)
	{
		let found: boolean = false;

		for (let destKey in dest)
		{
			if (srcKey == destKey)
			{
				found = true;

				break;
			}
		}

		if (found == false)
			dest[srcKey] = src[srcKey];
	}
}

/// Replace a keyword in a string with a value.
export function replaceKeyword (content: string, keyword: string, value: string): string
{
	content = content.replace (new RegExp (`%${keyword}%`, "g"), value);

	return (content);
}

/// Remove all whitespaces from a string.
export function removeWhitespaces (str: string): string
{
	str = str.replace (/\s/g, "");

	return (str);
}

/// Remove all content between two positions in a string.
export function removeStringBetween (str: string, start: number, end: number): string
{
	let temp: string = str.substring (0, start);
	temp += str.substring (end);

	return (temp);
}

/// Insert a string at a position.
export function insertString (original: string, insert: string, index: number): string
{
	let temp: string = "";
	let temp2: string = original.substring (index);

	temp = original.substring (0, index);
	temp += insert;
	temp += temp2;

	return (temp);
}

/// Get a command line keyword. For example --keyword
export function getCommandLineArgKeyword (arg: string): string
{
	let start: number = arg.indexOf ("=");
	let value: string = arg;

	if (start > -1)
		value = arg.substr (0, start);

	return (value);
}

/// Get the value associated with a command line keyword. For example --keyword=value
export function getCommandLineArgValue (arg: string): string
{
	let start: number = arg.indexOf ("=");
	let value: string = arg;

	if (start > -1)
	{
		let pos: number = arg.indexOf ("\"", start);

		if (pos > -1)
		{
			/// @fixme Add support for putting \" inside strings.
			let matches: string[] = arg.match (/\"(.*?)\"/gm);

			if (matches == null)
				throw new Error ("Quotations do not match up!");

			value = matches[0];

			// Remove the quotations around the value.
			value = value.substr (1, value.length - 2);
		}
		else
			value = arg.substr (1, arg.length);
	}

	return (value);
}

/// Handle the command line arguments.
export function commandLine (commands: any[]): void
{
	var helpHeader = `Bindergen
Copyright(c) 2018, FogChain, Corp
Under the MIT License\n\n`;

	commands.push ({
		cmd: ["help", "h"], 
		desc: "Help", 
		evt: function (args)
			{
				let str = helpHeader;

				commands.forEach (function (val, index)
					{
						let line = "  ";
						let spaces = "";
						let maxSpaces = 4;

						for (let iIdx = 0; iIdx < val.cmd.length; iIdx++)
						{
							let cmd = val.cmd[iIdx];

							if (iIdx == 0)
								line += "--" + cmd;

							if (iIdx == 1)
								line += ", -" + cmd;
						}

						if (index == 2)
							maxSpaces--;

						for (let iIdx = 0; iIdx < maxSpaces; iIdx++)
							spaces += " ";

						line += spaces + val.desc;
						str += line + "\n";
					});

				console.log (str);
			}
	});

	function getCommand (cmdList, cmdName)
	{
		let foundCmd = null;

		cmdList.forEach (function (val, index)
						{
							let cmd = val.cmd[0];

							if (cmd == cmdName)
								foundCmd = val;
						});

		return (foundCmd);
	}

	var execFunc = null;
	let args = [];

	process.argv.forEach (function (val, index)
			{
				let valKey = getCommandLineArgKeyword (val);
				let valVal = getCommandLineArgValue (val);

				if (index >= 2)
				{
					commands.forEach (function (val2, index2)
						{
							for (let iIdx = 0; iIdx < val2.cmd.length; iIdx++)
							{
								let cmd = val2.cmd[iIdx];

								if (iIdx == 0)
									cmd = "--" + cmd;

								if (valKey == cmd)
								{
									if (val2.executeLast == true)
										execFunc = val2.evt;
									else
										val2.evt (valVal);
								}
							}
						});
				}
			});

	if (execFunc != null)
		execFunc (args);
	else
	{
		let helpCmd = getCommand (commands, "help");
		helpCmd.evt ();
	}
}