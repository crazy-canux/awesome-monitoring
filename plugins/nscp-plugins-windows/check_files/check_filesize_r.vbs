'Script to Monitor Windows file Sizes with Nagios
'Usage: check_filesize -p "path" -f "file(s)" -w <number> -c <critical>
'By Felipe Ferreira 10/2008 www.felipeferreira.net Version 1.5 for NAGIOS
'Modified by Zach Armstrong 10/2010
'Version 2.0
'This now checks for minimum file sizes instead of maximum sizes. 
'@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Const intOK = 0
Const intWarning = 1
Const intCritical = 2
Const intError = 3
Const intUnknown = 3
Dim intFileSize : intFileSize = 0
dim intWarningN,intCriticalN
Dim sFile
Dim outputmsg,outputmsgstats
Dim intExit
dim strScriptFile : strScriptFile = WScript.ScriptFullname
Dim argcountcommand
Dim arg(25)
Dim path   		'get from arg -p
'Dim arrFiles(10)	'should be populated by arg -f files

'@@@@@@@@@@@HANDLES THE ARGUMENTS@@@@@@@@@@@@@@@
GetArgs()
if ((UCase(wscript.arguments(0))="-H") Or (UCase(wscript.arguments(0))="--HELP")) and (argcountcommand=1) then
	help()
elseif(1 < argcountcommand < 6) then
	path = GetOneArg("-p")
	If instr(path,"/") then
	  path = replace(path,"/","\")
	end if
	strFiles = GetOneArg("-f")
	  arrFiles = split(strFiles,",")
	intWarningN = toBytes(GetOneArg("-w"))
	intCriticalN = toBytes(GetOneArg("-c"))
end if


'DEBUGGING:'
'wscript.echo "Warning = " & intWarningN & " - " & varType(intWarningN)
'wscript.echo "Critical = " & intCriticalN & " - " & varType(intCriticalN)

'@@@@@@@@@@@HANDLES THE WARN AND CRITI OUTPUT@@@@@@@@@@@@@@@
for each sFile in arrFiles
	CheckFolder path, sFile
	if intFileSize <> 0 then
		outputmsg = outputmsg & " - "  & sFile & " = " & toMegaBytes(intFileSize) & "(MB)"	
'Should be Warning or Critical if even just on file is high
	if intExit <> 2 and intExit <> 1 then
'DEBUGGING:
'wscript.echo "TESTCRIT: " & toMegaBytes(intFileSize) & " > "&  toMegaBytes(intCriticalN) & " = " & (intFileSize > intCriticalN)
'wscript.echo "TESTWARN: " & toMegaBytes(intFileSize) & " > "&  toMegaBytes(intWarningN) & " = " & (intFileSize > intWarningN)
		If (intFileSize < intWarningN) and (intFileSize > intCriticalN) Then
			outputmsgstats = "STATUS WARNING "		
			intExit = intWarning
		Elseif (intFileSize <= intCriticalN)  Then
			outputmsgstats = "STATUS CRITICAL "			
			intExit = intCritical
		Elseif intFileSize > intWarningN Then
			outputmsgstats = "STATUS OK "
			intExit = intOK
		end if  
	end if  'warn or critc
	elseif intFileSize = 0 then
		outputmsg = outputmsg & " - "  & sFile & " = " & toMegaBytes(intFileSize) & "(MB)"
		outputmsgstats = "STATUS CRITICAL "			
		intExit = intCritical
	end if ' FileSize condition
	intFileSize = 0
next

wscript.echo outputmsgstats & outputmsg
wscript.quit(intExit)

Function CheckFolder(objFolder,strFile) 
'Check the size of file requested and returns it to intFileSize global var
on error resume next

    Dim oFSO           'FileSystemObject
    Dim oFolder        'Handle to the folder
    Dim oSubFolders    'Handle to subfolders collection
    Dim oFileCollection 'All files of the folder
'Connect to folder object and files
    Set oFSO = CreateObject("Scripting.FileSystemObject")
'Checks if Folder exists
	If oFSO.FolderExists(objFolder) = False Then
		wscript.echo "Error Folder " & objFolder & " was not found!"        
		wscript.quit(intError)	
	else
		Set oFolder = oFSO.GetFolder(objFolder)
  		Set oFileCollection = oFolder.Files        'gets all files of current folder
	       'Walk through each file in this folder collection.
		For each oFile in oFileCollection 'Gets its size based on the name.
                If oFile.name = strFile Then
		    intFileSize = oFile.size
		end if	
		next
	End If	
end function

Function toGigaBytes(bytes) 
                toGigabytes = int(((bytes / 1024) /1024) / 1024) 
End Function 
Function toMegaBytes(bytes) 
                toMegabytes = int ((bytes / 1024) /1024) 
		toMegabytes = FormatNumber(toMegaBytes,2)
End Function
Function toBytes(Megabytes) 
                toBytes = int ((Megabytes * 1024) *1024) 
End Function
Function Help()
'Prints out help 	
		Dim str
  		str="Check Size of File(s) in supplied Path."&vbCrlF&vbCrlF
  		str=str&"cscript "& strScriptFile &" -p Path -f file(s) -w warningValue(MB) -c criticalValue(MB)"&vbCrlF
  		str=str&vbCrlF
  		str=str&"-h [--help]                 Help."&vbCrlF
  		str=str&"-p path                     Path where files are."&vbCrlF  
  		str=str&vbCrlF
  		str=str&"By Felipe Ferreira October 2008, version 1.5." & vbCrlF
  		wscript.echo str		
End Function
Function GetArgs()
'Get ALL arguments passed to the script
	On Error Resume Next		
	Dim i		
	argcountcommand=WScript.Arguments.Count		
	for i=0 to argcountcommand-1
		arg(i)=WScript.Arguments(i)
	next		
End Function
Function GetOneArg(strName)
	On Error Resume Next
	Dim i
	for i=0 to argcountcommand-1
		if (Ucase(arg(i))=Ucase(strName)) then
			GetOneArg=arg(i+1)
			Exit Function
		end if
	next		
End Function
