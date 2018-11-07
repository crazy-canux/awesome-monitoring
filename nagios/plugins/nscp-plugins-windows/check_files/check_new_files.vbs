'###############################################'
'check new files nagios plugin vicenten 20130617'
' with performance graph number of new files    '
' norman.vicente@gmail.com                      '
' free to use and modify for your needs         '
'###############################################'

'example directory to check the new files
sPath = "D:\logs"

Const intOK = 0
Const intWarning = 1
Const intCritical = 2
Const intUknown = 3

Dim perfdata, finaloutput, oFile

Set oFSO = CreateObject("Scripting.FileSystemObject")
Set oFolder = oFSO.GetFolder(sPath)
Set oFiles = oFolder.Files
If oFiles.Count > 0 Then

   ' within the last 24 hours 1440 seconds. If such a file is found, exit the loop.
   bolFileIsNewEnough = False  ' init value


   For Each oFile In oFiles
     On Error Resume Next
     dFileModDate = oFile.DateLastModified
     If Err.Number = 0 Then
       If DateDiff("n", dFileModDate, Now) < 1440 Then
         bolFileIsNewEnough = True
         Exit For
       End If
     End If
   Next
   On Error Goto 0
   If Not bolFileIsNewEnough Then
	   finaloutput = "CRITICAL: " & "at D:\logs" '& oFile.DateLastModified &
	   WScript.Echo finaloutput
	   Wscript.Quit(intCritical)
     Else
      perfdata = oFiles.Count
      finaloutput = "OK: " & oFile.Name & " " & oFile.DateLastModified & "| Number of files=" & perfdata & "0;0;0"
      WScript.Echo finaloutput
      Wscript.Quit(intOK)
   End if

Else
   WScript.quit(intUknown)
End If
