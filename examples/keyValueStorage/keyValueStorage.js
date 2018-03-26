// Initialize the key value storage database.
var storage = new RadJav.Db.KeyValueStorage ();

// Open the database.
storage.open ("./temp.db");

// Write "value" to a key with the name "k".
storage.write ("k", "value").then (function ()
	{
		// Once writing is complete, read from the k value. 
		// Pass the value to the data argument.
		storage.read ("k").then (function (data)
			{
				alert ("Complete! The following value was stored: " + data);
			});
	});

storage.close ();