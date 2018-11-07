'
'
' Check files under a given path
' ==============================
'
' based on example scripts fond in nsclient++/scripts directory
'
' Author: werner.fuerst@assmann.at  2010-12-21
'
' uses NagiosPluginCDbl.vbs  from nsclient++/scripts/lib
'       - modified Version of NagiosPlugin.vbs which came with nsclient++ vers. 0.3.8.76
'            - compares bounds to double values, so 10 comes after 9 (numeric sorting)
'            - bounds definition conforms to nagios plugin guidelines (http://nagiosplug.sourceforge.net/developer-guidelines.html#THRESHOLDFORMAT)
'
' modifications in NSC.ini:
' =========================
'
'
' in [modules]:
' CheckExternalScripts.dll
'
' in [NRPE]:
' allow_arguments=1
' allow_nasty_meta_chars=1
' allowed_hosts=x.x.x.x
'
' in [External Script]:
' allow_arguments=1
' allow_nasty_meta_chars=1
'
' in [Script Wrappings]:
' vbs=cscript.exe //T:30 //NoLogo scripts\lib\wrapperCDbl.vbs %SCRIPT% %ARGS%
'
' in [Wrapped Scripts]:
' check_files=check_files.vbs $ARG1$ $ARG2$ $ARG3$ $ARG4$ $ARG5$ $ARG6$ $ARG7$ $ARG8$ $ARG9$ $ARG10$ $ARG11$ $ARG12$ $ARG13$ $ARG14$
'
'
' nagios usage:
' =============
'
' define command{
'        command_name check_nrpe_external
'        command_line $USER1$/check_nrpe -H $HOSTADDRESS$ -c $ARG1$ -a $ARG2$
' }
'
' define service{
'        use                     generic-service
'        host_name               windowsxx
'        service_description     Backup DB2
'        check_command           check_nrpe_external!check_files!/path:"e:\\BACKUP\\DB" /namefilter:"DB2\.DAT" /expectmatch:1 /age:3 /selage:hour /warning:1: /critical:1: /size:9000000000 /weekdays:"2,3"
'} }
' give alarm when file DB2.DAT in e:\BACKUP\DB was not written until 3 o clock on Monday or Tuesday (alarm if count is less than 1, so threshold is set to 1:)
'
'
'
'        check_command           check_nrpe_external!check_files!/path:"d:\\journal" /searchdepth:1 /selage:ignore /warning:30 /critical:40
' give alarm if there are more than 30 files under d:\journal
'
'
'        check_command           check_nrpe_external!check_files!/path:"T:\\nfs\\interface1"  /searchdepth:2 /age:2d /warning:5: /critical:3: /selage:newer /seldat:created
' give alarm if there are fewer than 3 files which are newer than 2 days under t:\nfs\interface1, search only 1 subdir down, use the creation date for comparison
'
'
'
' args:
' =====
'
'
' warning: threshold warning 
'    alert if x...
'             10	         < 0 or > 10, (outside the range of {0 .. 10})
'             10:	         < 10, (outside {10 .. 8})
'             ~:10	       > 10, (outside the range of {-8 .. 10})
'             10:20	       < 10 or > 20, (outside the range of {10 .. 20})
'             @10:20	     >= 10 and <= 20, (inside the range of {10 .. 20})
' critical: threshold critical 
' namefilter: regular expressionon on which files have to match
' age: files have to be older/newer (see selage) than age
'    e.g.: 5d: 5 days, 4h: 4 hours, 10n: 10 Minutes, 90s: 90 seconds   
' selage: older/newer/hour/ignore
'    older/newer: count only files if the are older/newer
'    hour: alert if file is not written until hour
'    ignore: count files independent of their age 
' searchdepth: search down x subdirs (searchdepth:1 - do not go down in directory hierarchy)
' seldat: modified/created
'    modified: date, when file was written
'    created: date, when file was created
' size: if file is smaller than given size give a warning
' expectmatch: if less than expectmatch files correspond to namefilter give a warning
' weekdays: 
'    if selage:hour files have to be written only on weekdays (1:sunday, 2:monday, ...)
'    if selage:newer or selage:older and the timeunit of age is d (days), we add as many days to age as the last weekday is back
'    e.g.: weekdays:23456 files are written on Monday, Tuesday, Wednesday, Thursday, Friday
'

Const PROGNAME = "check_files"
Const VERSION = "0.1.0"

Dim verbose
verbose = 0

Dim lastfile
Dim lastdat
Dim lastsize
Dim intdif
Dim warnsize
Dim matchcount
Dim weekdays
Dim ageint
Dim ageunit
Dim agestring
Dim inlevel


' Default settings for script.
threshold_warning = 10
threshold_critical = 20
alias = "default"
agestring = "5d"    '5 days
ageint = 5
ageunit = "d"
selage = "newer"
namefilter = ""
searchdepth = 0
seldat = "modified"
size = 0
expectmatch = 0
weekdays = "1,2,3,4,5,6,7"

' Create the NagiosPlugin object
Set np = New NagiosPlugin

' Define what args that should be used
np.add_arg "path", "Path", 1
np.add_arg "namefilter", "Filename Filter", 0
np.add_arg "age", "Age", 0
np.add_arg "selage", "newer, older, hour, ignore", 0
np.add_arg "searchdepth", "depth of subdirs to search", 0
np.add_arg "seldat", "modified or created", 0
np.add_arg "size", "size", 0
np.add_arg "warning", "warning threshold", 0
np.add_arg "critical", "critical threshold", 0
np.add_arg "expectmatch", "expect at least x matches", 0
np.add_arg "weekdays", "1,2,3,... 1-Sun 2-Mon 3-Tue...", 0
np.add_arg "alias", "Alias", 0

' If we have no args or arglist contains /help or not all of the required arguments are fulfilled show the usage output,.
If Args.Count < 1 Or Args.Exists("help") Or np.parse_args = 0 Then
	WScript.Echo Args.Count
	np.Usage
End If

' If we define /warning /critical on commandline it should override the script default.
If Args.Exists("warning") Then threshold_warning = Args("warning")
If Args.Exists("critical") Then threshold_critical = Args("critical")
If Args.Exists("namefilter") Then namefilter = Args("namefilter")
If Args.Exists("age") Then agestring = Args("age")
If Args.Exists("selage") Then selage = Args("selage")
If Args.Exists("searchdepth") Then searchdepth = Cint(Args("searchdepth"))
If Args.Exists("seldat") Then seldat = Args("seldat")
If Args.Exists("size") Then size = CDbl(Args("size"))
If Args.Exists("expectmatch") Then expectmatch = CInt(Args("expectmatch"))
If Args.Exists("weekdays") Then weekdays = Args("weekdays")
If Args.Exists("alias") Then alias = Args("alias")

' Set the msg output to be used (OK/WARNING/CRITICAL/UNKNOWN will be applied automaticly)
np.set_thresholds threshold_warning, threshold_critical

' Set ageint and ageunit
Set reage = New RegExp
reage.IgnoreCase = False
reage.Pattern = "^([0-9]+)([dhns]*)$"     'd: days h:hours n:minutes s:seconds

Set ages = reage.Execute(agestring)

For Each age In ages
pt age & " > " & age.SubMatches(0) & " > " & age.SubMatches(1)
ageint = CInt(age.SubMatches(0))
ageunit = age.SubMatches(1)
If ageunit = "" Then
	If selage = "hour" Then
	ageunit = "h"
	Else
	ageunit = "d"
	End If
End If
Next

' add some days if this and the days before do not belong to the weekdays
If ageunit="d" And (selage="newer" Or selage="older") Then
date1 = Now()
date00 = DateValue(Now)
date10 = DateValue(Now)

For i = 0 To 7
	' datex: last day when the file should have been written according to weekdays
	datex = SubtractDate(date00,i)
	'date00 = DateValue(datex)
	wdnowx = WeekDay(datex)
	If Instr(weekdays,wdnowx) Then
	' exit for if we found the youngest weekday
	Exit For
	End If
Next 

datex0 = DateValue(datex)
diffd = DateDiff("d",datex0,date10)
ageint = ageint + diffd
End If

intdif=0
matchcount=0
warnsize = 0

' go down the hierarchy, in intdif we get the number of corresponding files
CheckSubdirs Args("path")

' get return code according to intdif files
return_code = np.check_threshold(CDbl(intdif))

last = ""
If Len(namefilter) Then
	last = "filter: " & namefilter
End If

' if there was only one file...
If matchcount = 1 Then
	last = last & " found one: " & lastfile & " " & lastdat & " " & lastsize
End If

' warning message if file too small or too few of them
message = ""
If warnsize = matchcount And warnsize > 0 Then
	If return_code = 0 Then
		return_code = 1
	End If
	message = " TOO SMALL!!! "
End If

If matchcount < expectmatch Then
	If return_code = 0 Then
		return_code = 1
	End If
	message = " NOT FOUND!!! "
End If

If selage = "ignore" Then
	agemessage = " count: " 
ElseIf selage = "newer" Or selage ="older" Then
	agemessage = selage & " than " & ageint & " " & ageunit & " : "
ElseIf selage = "hour" Then
	agemessage = " written before " & ageint & " o-clock on " & weekdays & " : "
Else
	agemessage = selage & ": " & age & " " & ageunit & " "
End If

' for the performance data format warning and critical thresholds (no :@~)
cintw=np.get_threshold_perfdat("warning")          'PerfDat(np.get_threshold("warning"))
cintc=np.get_threshold_perfdat("critical")

' if we watch only a single file we will see the size in the performance data
If expectmatch = 1 and matchcount = 1 Then
	perfdata = "size=" &  lastsize
Else
	perfdata = "count=" & Cint(intdif) & ";" & cintw & ";" & cintc
End If

msg = "Testing " & Replace(Args("path"),"\","/") & " " & last & " " & agemessage & intdif & " w:" & np.get_threshold("warning") & " c: " & np.get_threshold("critical") & " " & message & " |" & perfdata

' Nice Exit with msg and exitcode
np.nagios_exit msg, return_code


Sub CheckSubdirs(StartDir)
	Set FSO = CreateObject("Scripting.FileSystemObject")
	inlevel = 0

	' go down hierarchy
	ShowSubfolders FSO.GetFolder(StartDir)

	'Clean up
	Set FSO = Nothing
End Sub





Sub ShowSubFolders(Folder)
	'inlevel: we start with 1, so the startdir has level 1!
	inlevel = inlevel + 1
	pt "level: " & inlevel & " Folder: " & Folder.Name & " searchdepth: " & searchdepth
	If (inlevel > searchdepth) And (searchdepth > 0) Then
		' we are too deep!
		inlevel = inlevel - 1
		pt "level2: " & inlevel & " Folder: " & Folder.Name
		Exit sub
	End If
	pt "level3: " & inlevel & " Folder: " & Folder.Name
	ListFiles Folder
	If ((inlevel < searchdepth) And (searchdepth > 0)) Or (searchdepth = 0) Then
		For Each Subfolder in Folder.SubFolders
			' go further down
			ShowSubFolders Subfolder
		Next
	End If
	' leave level
	inlevel = inlevel - 1
End Sub



Sub ListFiles(Folder)
	Set colFiles = Folder.Files
	
	pt "in folder " & Folder.Name
	For Each File in colFiles
		matched = true
		'pt "namefilter: " &  namefilter & " len:" & Len(namefilter)
		' if we defined a namefilter, use it (regexp!)
		If Len(namefilter) > 0 Then
		
			filename = File.Name
		
			'pt "namefilter1: " &  filename
			Set re = new regexp
			re.IgnoreCase = True
			re.Pattern = namefilter
			
			'pt "namefilter2: " &  namefilter & " len:" & Len(namefilter)
			If re.Test(filename) <> true Then
				matched = false
			End If
		End If
		
		' now test the matched files against the age
		If matched = true Then

			' count the matched files (we warn later if there are fewer the expectmatch files)
			matchcount = matchcount + 1
			pt "match: " & matchcount & " file: " & File.name

			' Date2 can be the DateCeated (backup of old machine, which is not time synchronized, so the created date is the date when the file was backed up to our server)
			Date1 = Now()
			If seldat = "created" Then
				Date2 = File.DateCreated
			Else
				Date2 = File.DateLastModified
			End If 

			' time difference between file and now
			dif =  Cint(DateDiff(ageunit,Date2,Date1))

			'remember the last file checked (we use it later in the message)
			lastfile = File.name
			lastdat = Date2
			lastsize = File.Size

			' remember the count of files which where too small
			If CDbl(File.Size) < CDbl(size) Then
				warnsize = warnsize + 1
			End If

			'pt "c0 dif: " &  dif & " age: " & age & " selage: " & selage & " seldat: " & seldat 
			' count the numer of files which are newer/older than given age
			If dif < ageint And selage = "newer" Then
				intdif = intdif + 1
			ElseIf dif > ageint And selage = "older" Then
				intdif = intdif + 1
			ElseIf selage = "ignore" Then
				intdif = intdif + 1
			ElseIf selage = "hour" Then
			' if selage:hour check if file has been written not after age (here age is the hour)
				If isnotolder(File) Then
					intdif = intdif + 1
				End If
			End If
		End If
	Next
End Sub



Function isnotolder(File)
	date1 = Now()
	date2 = File.DateLastModified 
	date00 = DateValue(Now)
	date10 = DateValue(Now)
	date20 = DateValue(date2)


	For i = 0 To 7
	' datex: last day when the file should have been written according to weekdays
	datex = SubtractDate(date00,i)
	'date00 = DateValue(datex)
	wdnowx = WeekDay(datex)
	If Instr(weekdays,wdnowx) Then
		' exit for if we found the youngest weekday
		Exit For
	End If
	Next 


	' we also need the second youngest weekday, when the file should have been written (is this english?)

	date00 = datex

	For i = 1 To 7
		datex2 = SubtractDate(date00,i)
		wdnowx2 = WeekDay(datex2)
		If Instr(weekdays,wdnowx2) Then
			Exit For
		End If
	Next 

	diffdx = DateDiff("d",datex,Now)     ' days between expected write date and today 
	diffh0 = DateDiff("h",date10,Now)    ' hours since midnight
	diffd = DateDiff("d",date20,datex)   ' days between written and expect to be written
	diffd2 = DateDiff("d",date20,datex2) ' days between second oldest expected write day and today
	diff1 = DateDiff("h",date20,date2)   ' hour of write (we currently do not use it)

	' we expect the best
	iswritten = 1

	' today to be written an hour passed: should be written today!
	If diffdx = 0 And diffh0 > ageint And diffd > 0 Then
		iswritten = 0
	End If

	' today to be written an hour not passed: should be written at least on second oldest expected day
	If diffdx = 0 And diffh0 <= ageint And diffd2 > 0 Then
		iswritten = 0
	End If


	' it should have been written on a day before, diffd gives a positive value: expected day passed
	If diffdx > 0 And diffd > 0 Then
		pt "i31: " & diffdx & " " & diffd
		iswritten = 0
	End If


	' older the 7 days!
	If diffd > 7 Then
		pt "i0: " & diffd
		iswritten = 0
	End If

	' if none of the bad conditions matched, we still have iswritten=1
	isnotolder = iswritten
End Function


Function SubtractDate(datea,ii)
	dateb = DateAdd("d",-ii,datea)
	SubtractDate = dateb
End Function

Function pt(strMsg)
	if verbose = 1 then
		wscript.echo strMsg
	end if
end function

