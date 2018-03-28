var RadJav;
(function (RadJav) {
    var GUI;
    (function (GUI) {
        var IO = (function () {
            function IO() {
            }
            IO.isDir = function (path) { };
            IO.isFile = function (path) { };
            IO.mkdir = function (path) { };
            IO.deleteFile = function (path) { };
            return IO;
        }());
        (function (IO) {
            var SerialComm = (function () {
                function SerialComm() {
                }
                SerialComm.prototype.getPort = function () { };
                SerialComm.prototype.getBaud = function () { };
                SerialComm.prototype.getByteSize = function () { };
                SerialComm.prototype.getStopBits = function () { };
                SerialComm.prototype.getParity = function () { };
                SerialComm.prototype.open = function () { };
                SerialComm.prototype.isOpen = function () { };
                SerialComm.prototype.read = function (bufferSize) { };
                SerialComm.prototype.write = function (buffer, bufferSize) { };
                SerialComm.prototype.close = function () { };
                return SerialComm;
            }());
            IO.SerialComm = SerialComm;
            var TextFile = (function () {
                function TextFile() {
                }
                TextFile.writeTextToFile = function (path, content) { };
                TextFile.readEntireFile = function (path) { };
                return TextFile;
            }());
        })(IO = GUI.IO || (GUI.IO = {}));
    })(GUI || (GUI = {}));
})(RadJav || (RadJav = {}));
