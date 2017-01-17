'-------------------------------------------------------------------------------------------------------
'check_MostRecentFile.vbs
'
'author: Dirk Keamy
'date: 11/16/2010
'
'Description:
'	For a given directory, find the most recently modified file and check if its modification
'	date is older than [w] or [c] minutes.  Also check if the file size is at least [z] bytes.
'	Output values: 0=OK, 1=Warning, 2=critical, 3=unknown

'	This script can be useful for monitoring application log file names that change with the date.

'	Intended for use with Nagios check_nrpe / nsclient++.
'
'Arguments:
'	directory path
'	warning age	(in minutes)	warning failure if most recent file is [x] minutes old.
'	critical age (in minutes)	critical failure if most recent file is [y] minutes old.
'	file size (in bytes)		critical failure if file size is < [z] byes.
'sample execution: 
'	cscript  check_MostRecentFile.vbs c:\testdir\ 1440 2880 100
'
'Notes:
'	- I commented out the output file feature: it's not needed for the main purpose (but it may be useful).     
'	- Files with the "hidden" attribute are specifically excluded: this can be easily changed if needed.
'	
'-------------------------------------------------------------------------------------------------------

'On Error Resume Next
ON ERROR GOTO 0
Err.Clear  

  Dim fso, folder, files, OutputFile
  Dim argFolder, argWarning, argCritical, argSize
  Dim fName, fSize, fMod
  Dim fTop, fTopDate, fTopSize, fAge
  
  'Output values expected by Nagios
	CONST Exit_OK = 0
	CONST Exit_Warning = 1
	CONST Exit_Critical = 2
	CONST Exit_Unknown = 3

	
 'validate arguments
	  If Wscript.Arguments.Count <> 4 Then
		Wscript.Echo "Must enter 4 arguments: directory C W S; i.e. c:\testdir\ 1440 2880 100"
		wscript.Quit(Exit_Unknown)
	  End If
  
	  If NOT IsNumeric(Wscript.Arguments.Item(1)) Then
		wscript.echo "enter a valid Warning age (minutes): i.e. 1440" 
		wscript.Quit(Exit_Unknown)
	  End If
		
	  If NOT IsNumeric(Wscript.Arguments.Item(2)) Then
		wscript.echo "enter a valid Critical age (minutes): i.e. 2880" 
		wscript.Quit(Exit_Unknown)
	  End If 

	  If NOT IsNumeric(Wscript.Arguments.Item(3)) Then
		wscript.echo "enter a valid minimum file size (bytes): i.e. 100" 
		wscript.Quit(Exit_Unknown)
	  End If 
	
  argFolder = Wscript.Arguments.Item(0)
  argWarning = Abs(Wscript.Arguments.Item(1))	'Abs: absolute value required for comparison later 
  argCritical = Abs(Wscript.Arguments.Item(2))	
  argSize = Abs(Wscript.Arguments.Item(3))	
  
  Set fso = CreateObject("Scripting.FileSystemObject")
  
  ''create an empty output file for saving output later on.
  'Set OutputFile = fso.CreateTextFile(argFolder&"\FileList.txt", True)
    
  'Check if directory exists before attempting to open 
  If fso.FolderExists(argFolder) Then
	Set folder = fso.GetFolder(argFolder)
	Set files = folder.Files
  Else
	wscript.echo "Invalid directory: " & argFolder 
	wscript.Quit(Exit_Unknown)
  End If
 
  'Check if directory has at least one file before going any further
  If folder.Files.Count = 0 Then
	wscript.echo "Empty directory: " & argFolder 
	wscript.Quit(Exit_Critical)
  End If
 
  'loop through array of files and save name, modified date, and file size.
  For each fileIdx In files
  		'Filter out files with "hidden" attribute:  constant=Hidden; value=2
		If (NOT fileIdx.Attributes and 2) Then
		fName = fileIdx.Name
		fMod  = fileIdx.DateLastModified 
		fSize = fileIdx.Size

		''write data to output file.
		'OutputFile.WriteLine(folderIdx.Name & vbTab & fMod & vbTab & fSize)
		
		''write data to console.
		'Wscript.echo(fName & vbTab & fMod & vbTab & fSize)

		'compare each file with one another and float most recently modified file to the top.
		If fMod > fTopDate Then
			fTop = fName
			fTopDate = fMod
			fTopSize = fSize
		End If
	End If
  Next
  
  'Determine age, in minutes, of most recent file: DateDiff interval: yyyy-year, m-month, d-day, h=hour, n=minute, s=second
  fAge = Abs(DateDiff("n", fTopDate, Now)) 
  
  'Determine if file exceeds either warning or critical levels
  If fAge > argCritical Then
		Wscript.echo("critical: " & fTop & " is " & fAge & " minutes old.")  
		Wscript.Quit(Exit_Critical)
  elseif fAge > argWarning Then
		Wscript.echo("warning: " & fTop & " is " & fAge & " minutes old.")
		Wscript.Quit(Exit_Warning)
  elseif fTopSize < argSize Then
		Wscript.echo("critcal: " & fTop & " is only " & fTopSize & " bytes.")
		Wscript.Quit(Exit_Critical)
  else
		Wscript.echo("pass: " & fTop & " is " & fAge & " minutes old.") 
		Wscript.Quit(Exit_OK)
  End If

  'Wscript.echo(vbLf & "current date & time: " & Now )
  'Wscript.echo(vbLf & "Most recent file: " & fTop & vbTab & fTopDate)
  
  'clean up
  OutputFile.Close
  Set fso = Nothing
  Set folder = Nothing
  Set files = Nothing
  
  ' The following line should never be executed / reached
    Wscript.Quit(Exit_Unknown) 
    Wscript.echo(vbLf & "Happy days :)")
	
  'End of script!
  'Wscript.echo(vbLf & "Happy days :)")

  