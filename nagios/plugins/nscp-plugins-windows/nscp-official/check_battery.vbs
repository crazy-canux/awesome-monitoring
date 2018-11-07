' =========================================================
' WMI script to return the charge remaining in a laptop battery, using the 
' EstimatedChargeRemaining property of the Win32_Battery class
' =========================================================

' Required Variables
Const PROGNAME = "check_battery"
Const VERSION = "0.0.1"

' Default settings for your script.
threshold_warning = "50:"
threshold_critical = "20:"
strComputer = "."

' Create the NagiosPlugin object
Set np = New NagiosPlugin

' Define what args that should be used
np.add_arg "computer", "Computer name", 0
np.add_arg "warning", "warning threshold", 0
np.add_arg "critical", "critical threshold", 0

' If we have no args or arglist contains /help or not all of the required arguments are fulfilled show the usage output,.
If Args.Exists("help") Then
	np.Usage
End If

' If we define /warning /critical on commandline it should override the script default.
If Args.Exists("warning") Then threshold_warning = Args("warning")
If Args.Exists("critical") Then threshold_critical = Args("critical")
If Args.Exists("computer") Then strComputer = Args("computer")
np.set_thresholds threshold_warning, threshold_critical

Set colInstances = np.simple_WMI_CIMV2(strComputer, "SELECT * FROM Win32_Battery")
return_code = OK

For Each objInstance In colInstances
	if message <> "" then : message = message & ", "
	if perf <> "" then : perf = perf & ", "
	message = message & "Battery " & objInstance.Status & " - Charge Remaining = " & objInstance.EstimatedChargeRemaining & "%"
	perf = perf & "charge=" & objInstance.EstimatedChargeRemaining
	return_code = np.escalate_check_threshold(return_code, objInstance.EstimatedChargeRemaining)
Next
' Nice Exit with msg and exitcode
np.nagios_exit message & "|" & perf, return_code
