import { Bindergen } from "./Bindergen";
import { Generator } from "./Generator";
import { GeneratorReference } from "./GeneratorReference";
import { GeneratorClass } from "./GeneratorClass";
import { GeneratorFunction } from "./GeneratorFunction";
import { GeneratorFunctionArgument } from "./GeneratorFunctionArgument";
import * as utils from "./utils";
import * as cheerio from "cheerio";
import { GeneratorOutput } from "./GeneratorOutput";
import { GeneratorClass } from "./GeneratorClass";
import { GeneratorFunction } from "./GeneratorFunction";
import { Block } from "./Block";

function fixGenericHTML (str: string): string
{
	str = str.replace (/\&\lt\;/g, "<");
	str = str.replace (/\&\gt\;/g, ">");

	return (str);
}

function getJNISignatureDataType (dataType: string)
{
	dataType = utils.removeWhitespaces (dataType);
	dataType = fixGenericHTML (dataType);
	dataType = dataType.replace (/\<(.*?)\>/g, "");
	let pos: number = dataType.indexOf ("final ");

	if (pos > -1)
		dataType = dataType.replace (/(s|final )/g, "");

	if (dataType == "void")
		return ("V");

	if (dataType == "boolean")
		return ("Z");

	if (dataType == "byte")
		return ("B");

	if (dataType == "char")
		return ("C");

	if (dataType == "short")
		return ("S");

	if (dataType == "int")
		return ("I");

	if (dataType == "long")
		return ("J");

	if (dataType == "float")
		return ("F");

	if (dataType == "double")
		return ("D");

	let str: string = "";
	pos = dataType.indexOf ("[]");

	if (pos > -1)
	{
		dataType = dataType.substring (0, pos - 1);
		str = "[" + getJNISignatureDataType (dataType);
	}

	str = `L${dataType};`;

	return (str);
}

var jni = new Generator ("AndroidJNI");
var reference: GeneratorReference = new GeneratorReference ();
reference.path = Bindergen.getArg ("android-docs");
reference.on ("classReference", async function (genClass: GeneratorClass)
	{
		let classes: string[] = GeneratorClass.getClassFromString (genClass.getName());
		let path: string = "";

		for (let iIdx = 0; iIdx < classes.length; iIdx++)
		{
			if (iIdx == (classes.length - 1))
				path += classes[iIdx] + ".html";
			else
				path += classes[iIdx] + "/";
		}

		try
		{
			let contents: string = await this.lookup ("/reference/" + path);

			let $ = cheerio.load (contents);

			for (let passes = 0; passes < 2; passes++)
			{
				let skipFirst: boolean = true;
				let methods = null;

				if (passes == 0)
					methods = $("#pubmethods tr");

				if (passes == 1)
				{
					methods = $("#inhmethods tr table tr");
					skipFirst = false;
				}

				for (let iIdx = 0; iIdx < methods.length; iIdx++)
				{
					if (skipFirst == true)
					{
						if (iIdx == 0)	// Skip the Public Methods table header.
							continue;
					}

					let returnType = $(methods[iIdx]).find ("td code")[0];
					let method = $(methods[iIdx]).find ("td code")[1];
					let methodName = $(method).find ("a")[0];

					let temp = $(returnType).find ("a");

					if (temp.length > 0)
						returnType = temp;

					let returnTypeHTML = $(returnType).html ();

					let func: GeneratorFunction = new GeneratorFunction (genClass);
					func.on ("generate", function (type: string, data: string)
						{
							let content: string = "";

							if (type == "jniHeaders")
								content = `static jmethodID native${this.parentClass.$typeName}${this.name};\n`;

							if (type == "jniCreate")
								content = `native${this.parentClass.$typeName}${this.name} = env->GetMethodID(${data}, "${this.name}", "${this.data}");\n`;

							return (content);
						});

					func.name = utils.removeWhitespaces ($(methodName).html ());

					if (returnTypeHTML.search (/(s|final\s)/g) > -1)
						returnTypeHTML = returnTypeHTML.replace (/(s|final\s)/g, "");

					func.returns = utils.removeWhitespaces (returnTypeHTML);

					let args: string = $(method).html ();
					let argsStart = args.indexOf ("(");
					let argsEnd = args.indexOf (")");
					let argsStr: string = args.substring (argsStart, argsEnd + 1);
					let realArgs: string[] = argsStr.match (/(?<=[\(,])[^\),]*(,)*()/g);

					for (let iJdx = 0; iJdx < realArgs.length; iJdx++)
					{
						let tempStr: string = realArgs[iJdx];
						let foundArg: string = "";

						foundArg = fixGenericHTML (tempStr);
						func.arguments.push (new GeneratorFunctionArgument ("", foundArg));
					}

					// Create the JNI signature.
					func.data = "(";

					for (let iJdx = 0; iJdx < func.arguments.length; iJdx++)
					{
						let arg: GeneratorFunctionArgument = func.arguments[iJdx];

						func.data += getJNISignatureDataType(arg.dataType);
					}

					func.data += ")" + getJNISignatureDataType (returnTypeHTML);
					genClass.addFunction (func);
				}
			}
		}
		catch (ex)
		{
			throw new Error (ex.message);
		}
	});
jni.referFrom (reference);
jni.createKeyword ("jniType", "");

let jniHeaders: GeneratorOutput = jni.createOutput ("jniHeaders");
let jniCreate: GeneratorOutput = jni.createOutput ("jniCreate");
jniHeaders.on ("first", function (output: GeneratorOutput)
	{
		let content = `
		// --- AUTOGENERATED CODE DO NOT MODIFY ---
		static bool hasJNILoaded;
`;
		return (content);
	});
jniCreate.on ("first", function (output: GeneratorOutput)
	{
		let content = `
		// --- AUTOGENERATED CODE DO NOT MODIFY ---`;

		return (content);
	});
jniCreate.on ("start", function ()
	{
		let content = `
		if (!hasJNILoaded)
		{
			Jni& jni = Jni::instance();
			JNIEnv* env = jni.getJniEnv();
			hasJNILoaded = true;

`;

		return (content);
	});
jniCreate.on ("end", function ()
	{
		let content = `
		}
`;

		return (content);
	});
jniHeaders.on ("classCreate", function (clss: GeneratorClass, ...args)
	{
		let content = `
		static jclass native${clss.$typeName}Class;
		static jmethodID native${clss.$typeName}Constructor;
`;

		return (content);
	});
jniCreate.on ("classCreate", function (clss: GeneratorClass, ...args)
	{
		let androidClassName: string = clss.getClassUsingSeparator ("/");

		let content = `
		native${clss.$typeName}Class = jni.findClass("${androidClassName}");
		native${clss.$typeName}Constructor = env->GetMethodID(native${clss.$typeName}Class, "<init>", "(Landroid/content/Context;)V");
`;

		return (content);
	});
jniHeaders.on ("functionCall", function (func, ...args)
	{
		let content = func.generate ("jniHeaders", `native${func.parentClass.$typeName}Class`);

		return (content);
	});
jniCreate.on ("functionCall", function (func, ...args)
	{
		let content = func.generate ("jniCreate", `native${func.parentClass.$typeName}Class`);

		return (content);
	});
jni.on ("inlineFunctionCall", function (block: Block, func: GeneratorFunction, ...args)
	{
		let argsStr: string = "";
		let argsAry: any[] = args[0];

		debugger;
		if ((argsAry.length - 1) > 0)
			argsStr = ", ";

		for (let iIdx = 0; iIdx < (argsAry.length - 1); iIdx++)
		{
			let arg: string = argsAry[iIdx];

			if (iIdx == (argsAry.length - 2))
				argsStr += arg;
			else
				argsStr += `${arg}, `;
		}

		let content: string = `env->CallNonvirtualVoidMethod(widget, native${func.parentClass.$typeName}Class, native${func.parentClass.$typeName}${func.name}${argsStr});`;

		return (content);
	});

module.exports = jni;