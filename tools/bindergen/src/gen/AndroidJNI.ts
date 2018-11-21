import { Bindergen } from "./Bindergen";
import { Generator } from "./Generator";
import { CustomFile } from "./CustomFile";

var jni = new Generator ("AndroidJNI");
jni.referFrom (Bindergen.getArg ("android-docs"));

module.exports = jni;