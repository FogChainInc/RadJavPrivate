//// [moduleMemberWithoutTypeAnnotation2.jump]
module JumpScript {
    export module CompilerDiagnostics {

        export interface IDiagnosticWriter {
            Alert(output: string): void;
        }

        export var diagnosticWriter = null;

        export function Alert(output: string) {
            if (diagnosticWriter) {
                diagnosticWriter.Alert(output);
            }
        }
    }
}


//// [moduleMemberWithoutTypeAnnotation2.js]
var JumpScript;
(function (JumpScript) {
    var CompilerDiagnostics;
    (function (CompilerDiagnostics) {
        CompilerDiagnostics.diagnosticWriter = null;
        function Alert(output) {
            if (CompilerDiagnostics.diagnosticWriter) {
                CompilerDiagnostics.diagnosticWriter.Alert(output);
            }
        }
        CompilerDiagnostics.Alert = Alert;
    })(CompilerDiagnostics = JumpScript.CompilerDiagnostics || (JumpScript.CompilerDiagnostics = {}));
})(JumpScript || (JumpScript = {}));
