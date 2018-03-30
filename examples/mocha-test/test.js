include ("./mocha.min.js").then (function ()
	{
		sdfgsdfg ();
		mocha.setup ({
				ui: "bdd", 
				reporter: "json"
			});
		Mocha.process.stdout.write = function (str){alert (str);};

		describe('Array', function()
				{
					describe('#indexOf()', function()
						{
							it('should return -1 when the value is not present', function()
								{
										assert.equal([1,2,3].indexOf(4), -1);
								});
						});
				});

		mocha.run ();
	});