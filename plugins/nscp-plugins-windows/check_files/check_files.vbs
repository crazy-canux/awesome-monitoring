'===============================================================================
' Script Name  : check_files.vbs
' Usage Syntax : cscript.exe //NoLogo //T:10 check_filecount.vbs -p <path> [-r] [-f <filter>] -w <warn> -c <crit> | -h
' Author       : Alex Simenduev, PlanetIT (http://www.planetit.ws)
' Version      : 2009.11.1
' LastModified : 29/10/2009
' Description  : Counts the number of files in given directory and its
'                subdirectories, with optional filters applied. The output is
'                compatible with Nagios. Default Exit code is 3 (STATE_UNKNOWN)
'
' License:
' - This script is supplied as-is without any support, I hope it works for you.
' - You free to modify/distribute this script as you wish, but you must have the
'   following line in the script:
'   Original author Alex Simenduev, PlanetIT (http://www.planetit.ws)
'
' Change Log:
'    Legend:
'       [*] Informational, [!] Bugix, [+] Added, [-] Removed
'
'    Version 2009.11.1:
'       [!] More accurate argument parsing, will fix some small bugs.
'       [!] Fixed bug in RegEx filter if in pattern a ':' sign was specified.
'       [+] Added check for invalid filter type, script will exit with error printed.
'       [+] Added check for invalid filter value, script will exit with error printed.
'       [+] Added examples to the help screen.
'       [*] Recursive check is optional now, use '-r' option to turn it on.
'
'    Version 2009.10.29:
'       [!] More accurate check for the '--path' argument, added message.
'
'    Version 2009.10.28:
'       [*] Initial release.
'===============================================================================
Option Explicit

' Script version Constant
Const VERSION = "2009.11.1"

' Nagios states Constants
Const STATE_OK          = 0
Const STATE_WARNING     = 1
Const STATE_CRITICAL    = 2
Const STATE_UNKNOWN     = 3
Const STATE_DEPENDENT   = 4

' Filter Constants
Const FILTER_KEY        = 0
Const FILTER_VALUE      = 1

' Global variables
Dim gaStates	: gaStates = Array("OK", "WARNING", "CRITICAL", "UNKNOWN", "DEPENDENT")
Dim giTimestamp : giTimestamp = UnixTime(Now)
Dim gbRecursive : gbRecursive = False
Dim gaFilters   : gaFilters = Array()
Dim gsPath      : gsPath = ""
Dim giWarn      : giWarn = ""
Dim giCrit      : giCrit = ""

' Create all requered global objects
On Error Resume Next
Dim StdOut : Set StdOut = Wscript.Stdout
Dim goFSO  : Set goFSO = CreateObject("Scripting.FileSystemObject")

' Do not continue if any of required objects failed to create
If Err.Number <> 0 Then 
    StdOut.WriteLine "Failed to create required VBScript objects."
    Wscript.Quit(STATE_UNKNOWN)
End If
On Error GoTo 0
' End of "Create all requered global objects"

' Call a Main function and exit the script
Wscript.Quit(Main())

Function Main() : Main = STATE_UNKNOWN
    Dim strArg, strNextArg, intFileCount

    ' Set default values for variables
    strArg = ""
    strNextArg = ""

    ' If no arguments were specified, then print usage and exit
    If (Wscript.Arguments.Count = 0) Then
        PrintUsage()
        Exit Function
    End If
    
    ' Now let's parse the command-line/config options
    For Each strArg in Wscript.Arguments
        If (strNextArg = "") Then
            Select case LCase(strArg)
                case "-h", "--help"
                    Call PrintHelp()
                    Exit Function

                case "-r", "--recursive"
                    gbRecursive = True               

                case "-p", "--path"
                    strNextArg = "path"                    
                    
                case "-f", "--filter"
                    strNextArg = "filter"

                case "-w", "--warn"
                    strNextArg = "warning"

                case "-c", "--crit"
                    strNextArg = "critical"
                
                case else
                    ' Ignore all unknown arguments
                    Stdout.Writeline "Unknown argument '" & strArg & "', ignoring."
            End Select
        Else
            Select case strNextArg
                case "path"
                    strNextArg = ""                
                    If (goFSO.FolderExists(strArg)) Then
                        gsPath = CStr(strArg)
                    Else
                        Stdout.Writeline "Path '" & strArg & "' does not exists."
                    End If                    

            ' Collect filters
                case "filter"
                    strNextArg = ""                
                    Redim Preserve gaFilters(Ubound(gaFilters) + 1)
                    gaFilters(Ubound(gaFilters)) = CStr(strArg)

                case "warning"
                    strNextArg = ""                
                    If (IsNumeric(strArg)) Then giWarn = CInt(strArg)

                case "critical"
                    strNextArg = ""                
                    If (IsNumeric(strArg)) Then giCrit = CInt(strArg)
            End Select
        End If
    Next
   
    If (gsPath = "") Or (giWarn = "") Or (giCrit = "") Then
        StdOut.WriteLine "Missing or invalid arguments, check usage."
        Exit Function
    End If
    
    ' Run the check and save the result
    intFileCount =  FileCount(gsPath, gaFilters, gbRecursive)
    
    ' Proccess the result
    If intFileCount >= giCrit Then
        Main = STATE_CRITICAL
    ElseIf intFileCount >= giWarn Then        
        Main = STATE_WARNING
    Else
        Main = STATE_OK
    End If

    ' Output final message
    StdOut.WriteLine gaStates(Main) & ": " & intFileCount & " files were matched.|count=" & intFileCount
End Function

' Function Name : FileCount(pPath, pFilters, pRecursive)
' Return value  : int
' Description   : Counts files in given directory or all subdirectries.
Function FileCount(pPath, pFilters, pRecursive) : On Error Resume Next : FileCount = 0
    Dim objFile, objFolder, strFilter, arrFilter
    Dim intMatched, intAge
        
    ' Let's go over each file in the folder
    For Each objFile in goFSO.GetFolder(pPath).Files
        intMatched = 0
    
        ' Here we parsing all filters
        For Each strFilter in pFilters
            arrFilter = Split(strFilter, ":", 2)
            arrFilter(FILTER_KEY) = Ucase(trim(arrFilter(FILTER_KEY)))
            
            If (arrFilter(FILTER_KEY) = "REGEX") Then               
                If (RegexTest(arrFilter(FILTER_VALUE), objFile.Name) = True) Then intMatched = intMatched + 1
            Else
                Select Case arrFilter(FILTER_KEY)
                    Case "SIZE"
                        Execute("If objFile.Size " & arrFilter(FILTER_VALUE) & " Then intMatched = intMatched + 1")                        
                    
                    Case "AGECREATED"
                        intAge = giTimestamp - UnixTime(objFile.DateCreated)
                        Execute("If intAge " & arrFilter(FILTER_VALUE) & " Then intMatched = intMatched + 1")

                    Case "AGEMODIFIED"
                        intAge = giTimestamp - UnixTime(objFile.DateLastModified)
                        Execute("If intAge " & arrFilter(FILTER_VALUE) & " Then intMatched = intMatched + 1")
                        
                    Case "AGEACCESSED"
                        intAge = giTimestamp - UnixTime(objFile.DateLastAccessed)                    
                        Execute("If intAge " & arrFilter(FILTER_VALUE) & " Then intMatched = intMatched + 1")
                    
                    Case Else
                        StdOut.WriteLine "Invalid filter was specified, check usage."
                        Wscript.Quit(STATE_UNKNOWN)
                End Select
                
                ' Do not continue if any error were found
                ' Will prevent to put invalid 'FILTER_VALUE' value of filters
                If Err.Number <> 0 Then
                    StdOut.WriteLine Err.Source & ", " & Err.Description
                    Wscript.Quit(STATE_UNKNOWN)
                End if
            End If
        Next
        
        ' Increment if decideed so by the above checks.
        ' Ubound(pFilters) will be '-1' if the array is empty
        If (intMatched = Ubound(pFilters) + 1) Or (Ubound(pFilters) = -1) Then
            FileCount = FileCount + 1
        End If        
    Next
       
    ' Do a recursive check if asked so
    If (pRecursive = True) Then
        ' Here comes the magic of recursive function!
        For Each objFolder In goFSO.GetFolder(pPath).SubFolders
            FileCount = FileCount + FileCount(objFolder.Path, pFilters, True)
        Next
    End If
    
    Set objFile = Nothing
    Set objFolder = Nothing
End Function

' Function Name : RegexTest(pArguments)
' Return value  : boolean
' Description   : Test string against given Regular expression pattern.
' For more info : http://msdn.microsoft.com/en-us/library/ms974570.aspx
Function RegexTest(pPattern, pString) : On Error Resume Next
    Dim objRegex : Set objRegex = New regexp ' Create the RegExp object
    
    ' Set options for regex
    objRegex.Pattern = pPattern
    objRegex.Global = True
    objRegex.IgnoreCase = True

    RegexTest = objRegex.Test(pString)
    
    Set objRegex = Nothing
End Function

' Function Name : UnixTime(pDate)
' Return value  : long
' Description   : Convert Date to Unix timestamp format
Function UnixTime(pDate)
    UnixTime = CLng(DateDiff("s", "01/01/1970 00:00:00", pDate))
End Function

' This sub prints a usage text
Sub PrintUsage()
    StdOut.WriteLine "Usage: " & Wscript.ScriptName & " -p <path> [-r] [-f <filter>] -w <warn> -c <crit> | -h"
End Sub

' This sub prints a help message along with usage
Sub PrintHelp()
    Stdout.WriteLine "Check_Files - Nagios NRPE plugin for windows, ver. " & VERSION
    StdOut.WriteLine "Copyright (C) 2009, Alex Simenduev" & vbNewLine

    Call PrintUsage()
    StdOut.WriteLine    
    StdOut.WriteLine "Command-line options:"
    StdOut.WriteLine "-p, --path <path>     : folder path"
    StdOut.WriteLine "-r, --recursive       : check directories recursively"
    StdOut.WriteLine "-f, --filter <filter> : add filter to files, can be specifed more than once"
    StdOut.WriteLine "-w, --warn <warn>     : set WARNING state"
    StdOut.WriteLine "-c, --crit <crit>     : set CRITICAL state"
    StdOut.WriteLine "-h, --help            : print this help message"  & vbNewLine

    StdOut.WriteLine "Avaliable filters:"
    StdOut.WriteLine "   Size, AgeCreated, AgeAccessed, AgeModified, RegEx"  & vbNewLine
    
    StdOut.WriteLine "Examples:"
    Stdout.WriteLine "   "  & Wscript.ScriptName & " -p C:\Windows\Temp -w 100 -c 200"
    Stdout.WriteLine "   Count files in C:\Windows\Temp, return WARNING if number of"
    Stdout.WriteLine "   files more than 100 and CRITICAL if more than 200." & vbNewLine

    Stdout.WriteLine "   "  & Wscript.ScriptName & " -p C:\Windows\Temp -f ""size:>1024*1024"" -w 5 -c 20"
    Stdout.WriteLine "   Count files in D:\Files, and look only for files that are bigger than 1MB,"
    Stdout.WriteLine "   return WARNING if number of files more than 5 and CRITICAL if more than 20." & vbNewLine
    
    Stdout.WriteLine "   Will add more soon."
End Sub
