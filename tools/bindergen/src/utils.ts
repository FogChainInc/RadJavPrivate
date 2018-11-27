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
				return func.apply(this, aryArgs);
			else
				return func.apply(context, aryArgs);
		};

	return objReturn;
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