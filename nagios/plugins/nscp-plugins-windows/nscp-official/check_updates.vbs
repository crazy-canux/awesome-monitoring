' =========================================================
' Script to check for updates onwwindows machines.
' Shamelessly stolen from Micha³ Jankowski (fooky@pjwstk.edu.pl) script.
' =========================================================

' Required Variables
Const PROGNAME = "check_updates"
Const VERSION = "0.0.1"

' Default settings for your script.
threshold_warning = 50
threshold_critical = 20

' Create the NagiosPlugin object
Set np = New NagiosPlugin

' Define what args that should be used
np.add_arg "warning", "warning threshold", 0
np.add_arg "critical", "critical threshold", 0

' If we have no args or arglist contains /help or not all of the required arguments are fulfilled show the usage output,.
If Args.Exists("help") Then
	np.Usage
End If

' If we define /warning /critical on commandline it should override the script default.
If Args.Exists("warning") Then threshold_warning = Args("warning")
If Args.Exists("critical") Then threshold_critical = Args("critical")
np.set_thresholds threshold_warning, threshold_critical

Set objAutoUpdate = CreateObject("Microsoft.Update.AutoUpdate")

intResultDetect = objAutoUpdate.DetectNow
If intResultDetect <> 0 Then: np.nagios_exit "UNKNOWN: Unable to detect Automatic Updates.", UNKNOWN

Set objSession = CreateObject("Microsoft.Update.Session")
Set objSearcher = objSession.CreateUpdateSearcher

intUncompleted = 0
intUncompletedSoftware = 0

Set objSysInfo = CreateObject("Microsoft.Update.SystemInfo")
If objSysInfo.RebootRequired Then: np.nagios_exit "WARNING: Reboot required.", WARNING

Set result = objSearcher.Search("IsInstalled = 0 and IsHidden = 0")
Set colDownloads = result.Updates

For i = 0 to colDownloads.Count - 1
	If colDownloads.Item(i).AutoSelectOnWebsites Then
		updatesNames = colDownloads.Item(i).Title & "+ " & updatesNames 
  		intUncompleted = intUncompleted + 1
	Else
		intUncompletedSoftware = intUncompletedSoftware + 1
	End If
Next

return_code = OK

If intUncompleted > 0 Then
	return_code = np.check_threshold(intUncompleted)
	np.nagios_exit "Number of critical updates not installed: " & intUncompleted & " <br />Number of software updates not installed: " & intUncompletedSoftware & " <br /> Critical updates name: " & updatesNames, return_code
Else
	np.nagios_exit "There is no critical updates <br />Number of software or driver updates not installed: " & intUncompletedSoftware, OK
End If
