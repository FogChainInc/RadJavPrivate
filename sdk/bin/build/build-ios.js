
const ncp = require("ncp").ncp;
const child = require('child_process');
const fs = require('fs');


let cwd = process.cwd();

let configName = "ios";
let configFile = "app.json";
let configFilePath = cwd+"/"+configFile;
let sdkPath = __dirname+"/../../sdk";
let sdkVmPath = sdkPath+"/vm-"+configName;
let vmPath = cwd+"/vm-"+configName;
let dependenciesPath = sdkPath+"/dependencies/"+configName;


const appConfigs = require(configFilePath);

let config = parse_config(appConfigs, configName);

inject_vm().then( function () {
	for (let i in config["architectures"])
	{
		generate_project(config["architectures"][i]);
	}
});

function generate_project(arch)
{
	let cmakeCommand = "cmake";
	let args = ["..",
				"-DRJ_PROJECT_NAME="+config["projectName"],
				"-DRJ_BUNDLE_IDENTIFIER="+config["bundleIdentifier"],
				"-DRJ_BUNDLE_SIGNATURE="+config["bundleSignature"],
				"-DRJ_CODESIGN_IDENTITY="+config["codesignIdentity"],
				"-DRJ_DEVELOPMENT_TEAM_ID="+config["developmentTeamID"],
				"-DRJ_PROVISIONING_PROFILE_NAME="+config["provisioningProfileName"],
				"-DRJ_DEPENDENCIES_PATH="+dependenciesPath,
				"-DRJ_ARCHITECTURE="+arch,
				"-DRJ_APP_PATH="+cwd+"/app",
				"-DCMAKE_TOOLCHAIN_FILE="+vmPath+"/CMake/toolchain/iOS.cmake",
				"-G",
				"Xcode"
	];
	
	let buildDir = vmPath+"/build-"+arch;
	
	if (!fs.existsSync(buildDir)){
		fs.mkdirSync(buildDir);
	}
	
	console.log("Generating Xcode project for "+config["name"]+"("+arch+") platform");
	
	try {
		child.execFileSync(cmakeCommand, args, {cwd: buildDir});
	}
	catch (err)
	{
		console.log("There was an error during Xcode project generation");
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
		console.log("Missing configs")
		exit(1);
	}
	
	let tmpConfig = cfgObj["configs"].find(x => x.name === platform);
	if (isUndefined(tmpConfig))
	{
		console.log("Missing configuration for "+platform+" platform");
		exit(1);
	}
	
	let cfg = tmpConfig;

	if (isUndefined(cfgObj["projectName"]))
	{
		console.log("Missing projectName in app.json");
		exit(1);
	}
	
	cfg["projectName"] = cfgObj["projectName"];
	
	return cfg;
}
