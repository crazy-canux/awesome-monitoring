' =========================================================
' WMI script to return the charge remaining in a laptop battery, using the 
' EstimatedChargeRemaining property of the Win32_Battery class
' =========================================================

' Required Variables
Const PROGNAME = "check_printer"
Const VERSION = "0.0.1"

' Default settings for your script.
threshold_warning = 50
threshold_critical = 20
strComputer = "."
strPrinter = ""

' Create the NagiosPlugin object
Set np = New NagiosPlugin

' Define what args that should be used
np.add_arg "computer", "Computer name", 0

' If we have no args or arglist contains /help or not all of the required arguments are fulfilled show the usage output,.
If Args.Exists("help") Then
	np.Usage
End If

' If we define /warning /critical on commandline it should override the script default.
If Args.Exists("computer") Then strComputer = Args("computer")

Set colInstances = np.simple_WMI_CIMV2(strComputer, "SELECT * FROM Win32_Printer")
return_code = OK

perf = ""
msg = ""
For Each objInstance In colInstances
	msg = msg & "" & objInstance.Caption & _
		" {Status: " & printer_status(objInstance.PrinterStatus) & _
		", State: " & printer_state(objInstance.PrinterState) & "}; "
	perf = perf & "status=" & objInstance.PrinterStatus & " state=" & objInstance.PrinterState & " "
	return_code = np.escalate(return_code, check_status(objInstance.PrinterStatus))
	return_code = np.escalate(return_code, check_state(objInstance.PrinterState))
Next

' Nice Exit with msg and exitcode
np.nagios_exit msg, return_code


Public Function printer_status(code)
	Select Case code
		case 1: printer_status = "Other"
		case 2: printer_status = "Unknown"
		case 3: printer_status = "Idle"
		case 4: printer_status = "Printing"
		case 5: printer_status = "WarmUp"
		case 6: printer_status = "Stopped Printing"
		case 7: printer_status = "Offline"
		case else: printer_status = "Undefined"
	End Select
End Function

Public Function check_status(code)
	Select Case code
		case 1: check_status = OK
		case 2: check_status = UNKNOWN
		case 3: check_status = OK
		case 4: check_status = OK
		case 5: check_status = OK
		case 6: check_status = OK
		case 7: check_status = CRITICAL
		case else: check_status = UNKNOWN
	End Select
End Function

Public Function printer_state(code)
	Select Case code
		case 0: printer_state = "Paused"
		case 1: printer_state = "Error"
		case 2: printer_state = "PendingDeletion"
		case 3: printer_state = "PaperJam"
		case 4: printer_state = "PaperOut"
		case 5: printer_state = "ManualFeed"
		case 6: printer_state = "PaperProblem"
		case 7: printer_state = "Offline"
		case 8: printer_state = "IOActive"
		case 9: printer_state = "Busy"
		case 10: printer_state = "Printing"
		case 11: printer_state = "OutputBinFull"
		case 12: printer_state = "NotAvailable"
		case 13: printer_state = "Waiting"
		case 14: printer_state = "Processing"
		case 15: printer_state = "Initialization"
		case 16: printer_state = "WarmingUp"
		case 17: printer_state = "TonerLow"
		case 18: printer_state = "NoToner"
		case 19: printer_state = "PagePunt"
		case 20: printer_state = "UserInterventionRequired"
		case 21: printer_state = "OutofMemory"
		case 22: printer_state = "DoorOpen"
		case 23: printer_state = "Server_Unknown"
		case 24: printer_state = "PowerSave"
		case else: printer_state = "Undefined"
	End Select
End Function

Public Function check_state(code)
	Select Case code
		case 0: check_state = OK
		case 1: check_state = CRITICAL
		case 2: check_state = OK
		case 3: check_state = WARNING
		case 4: check_state = CRITICAL
		case 5: check_state = OK
		case 6: check_state = CRITICAL
		case 7: check_state = CRITICAL
		case 8: check_state = OK
		case 9: check_state = OK
		case 10: check_state = OK
		case 11: check_state = WARNING
		case 12: check_state = UNKNOWN
		case 13: check_state = OK
		case 14: check_state = OK
		case 15: check_state = OK
		case 16: check_state = OK
		case 17: check_state = WARNING
		case 18: check_state = CRITICAL
		case 19: check_state = OK
		case 20: check_state = CRITICAL
		case 21: check_state = CRITICAL
		case 22: check_state = WARNING
		case 23: check_state = CRITICAL
		case 24: check_state = OK
		case else: check_state = UNKNOWN
	End Select
End Function
