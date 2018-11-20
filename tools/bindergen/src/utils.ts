export function showError (err: string): void
{
	console.error (err);
}

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
				if (index == 2)
				{
					commands.forEach (function (val2, index2)
						{
							for (let iIdx = 0; iIdx < val2.cmd.length; iIdx++)
							{
								let cmd = val2.cmd[iIdx];

								if (iIdx == 0)
									cmd = "--" + cmd;

								if (val == cmd)
									execFunc = val2.evt;
							}
						});
				}

				if (index > 2)
					args.push (val);
			});

	if (execFunc != null)
		execFunc (args);
	else
	{
		let helpCmd = getCommand (commands, "help");
		helpCmd.evt ();
	}
}