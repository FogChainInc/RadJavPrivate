(async function ()
{
	class GObject
	{
		constructor ()
		{
			this._appObj = { type: "external" };
		}

		create ()
		{
			return (null);
		}

		setText (text)
		{
			return (null);
		}
	}

	let embed = Bindergen.useGenerators (["RadJavGenerator", "V8"]);
	embed.addClass ("GObject", GObject);
}());