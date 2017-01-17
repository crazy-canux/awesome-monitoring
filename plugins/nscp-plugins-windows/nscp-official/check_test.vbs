' =========================================================
' Example file of setting up a script to use NagiosPlugin.vbs as base.
' =========================================================

' Required Variables
Const PROGNAME = "check_test"
Const VERSION = "0.9.0"

' Default settings for your script.
threshold_warning = 10
threshold_critical = 20
alias = "default"

' Create the NagiosPlugin object
Set np = New NagiosPlugin

' Define what args that should be used
np.add_arg "arg", "Argument", 1
np.add_arg "warning", "warning threshold", 0
np.add_arg "critical", "critical threshold", 0
np.add_arg "alias", "Alias", 0

' If we have no args or arglist contains /help or not all of the required arguments are fulfilled show the usage output,.
If Args.Count < 1 Or Args.Exists("help") Or np.parse_args = 0 Then
	WScript.Echo Args.Count
	np.Usage
End If

' If we define /warning /critical on commandline it should override the script default.
If Args.Exists("warning") Then threshold_warning = Args("warning")
If Args.Exists("critical") Then threshold_critical = Args("critical")
If Args.Exists("alias") Then alias = Args("alias")
np.set_thresholds threshold_warning, threshold_critical

' Set the msg output to be used (OK/WARNING/CRITICAL/UNKNOWN will be applied automaticly)
return_code = np.check_threshold(Args("arg"))
msg = "Testing " & Args.Item("host") & " " & np.get_threshold("warning") & " " & np.get_threshold("critical")

' Nice Exit with msg and exitcode
np.nagios_exit msg, return_code
