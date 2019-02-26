
const ncp = require("ncp").ncp;
const child = require('child_process');
const fs = require('fs');
const rif = require('replace-in-file');


let cwd = process.cwd();

let configName = "android";
let configFile = "app.json";
let configFilePath = cwd+"/"+configFile;
let sdkPath = __dirname+"/../../sdk";
let sdkVmPath = sdkPath+"/vm-"+configName;
let vmPath = cwd+"/vm-"+configName;
let dependenciesPath = sdkPath+"/dependencies/"+configName;


const appConfigs = require(configFilePath);

let config = parse_config(appConfigs, configName);

inject_vm().then( function () {
	personalize_project();
});

function personalize_project()
{
	if (isUndefined(config["architectures"]) ||
		config["architectures"].length == 0)
	{
		console.log("Missing or invalid architectures section in "+configFile+" file");
		exit(1);
	}
	
	console.log("Generating Android Studio project");
	
	let architectures = JSON.stringify(config["architectures"]);
	architectures = architectures.slice(1, architectures.length-1);
	
	const substitutionOptions = {
		from: [/\${RJ_APP_PATH}/g,
			/\${RJ_PROJECT_NAME}/g,
			/\${RJ_APPLICATION_ID}/g,
			/\${RJ_DEPENDENCIES_PATH}/g,
			/\${RJ_ARCHITECTURES}/g,
			/\${RJ_MIN_SDK_VERSION}/g,
			/\${RJ_TARGET_SDK_VERSION}/g,
			/\${RJ_COMPILE_SDK_VERSION}/g
		],
		to: [cwd+"/app",
			config["projectName"],
			config["applicationId"],
			dependenciesPath,
			architectures,
			config["minSdkVersion"],
			config["targetSdkVersion"],
			config["compileSdkVersion"]
		]
	};
	
	let templateFiles = [vmPath+"/app/build.gradle",
						vmPath+"/app/src/main/res/values/strings.xml"];
	
	substitutionOptions["files"] = templateFiles;
	
	try {
		rif.sync(substitutionOptions);
	}
	catch(error)
	{
		console.log("Unable to generate Android Studio project");
		exit(1);
	}
}

function isUndefined(value)
{
	if (typeof value === "undefined")
		return true;
	else
		return false;
}

function exit(status)
{
	process.exit(status);
}

function inject_vm()
{
	return new Promise( function( resolve, reject) {
		ncp(sdkVmPath, vmPath, function (err) {
			if (err) {
				console.error(err);
				reject(err);
			}
			else {
				console.log('Virtual machine injected');
				resolve();
			}
		});
	});
}

function parse_config(cfgObj, platform)
{
	if (isUndefined(cfgObj["configs"]))
	{
		console.log("Missing configs in "+configFile+" file")
		exit(1);
	}
	
	let tmpConfig = cfgObj["configs"].find(x => x.name === platform);
	if (isUndefined(tmpConfig))
	{
		console.log("Missing configuration for "+platform+" platform in "+configFile+" file");
		exit(1);
	}
	
	let cfg = tmpConfig;

	if (isUndefined(cfgObj["projectName"]))
	{
		console.log("Missing projectName in "+configFile+" file");
		exit(1);
	}
	
	cfg["projectName"] = cfgObj["projectName"];
	
	return cfg;
}
