'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' TestRhino5Automation.rvb -- July 2016, Dale Fugier.
' If this code works, it was written by Dale Fugier.
' If not, I don't know who wrote it.
' Works with Rhinoceros 6.
'
' Run from a Windows command prompt with this syntax:
'
'   C:\> cscript TestRhino6Automation.vbs
'
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Option Explicit

' Main subroutine
Sub TestRhino6Automation

  ' Declare local variables
  Dim objRhino, objShell, strDesktop, strPath
  
  ' Create a Rhino application object
  On Error Resume Next
  Set objRhino = CreateObject("Rhino.Application")
  If Err.Number <> 0 Then
    Call WScript.Echo("Failed to create Rhino object.")
    Exit Sub
  End If
  Call WScript.Echo("Rhino object created.")

  ' Run some commands
  Call objRhino.RunScript("_Circle 0 10", 0)
  Call objRhino.RunScript("_Line -5,-5,0 5,5,0", 0)
  Call objRhino.RunScript("_Line 5,-5,0 -5,5,0", 0)
  Call WScript.Echo("Geometry created.")
  
  ' Create a shell object
  Set objShell = WScript.CreateObject("WScript.Shell")
  Call WScript.Echo("Shell object created.")
  
  ' Get the desktop folder
  strDesktop = objShell.SpecialFolders("Desktop")
  strPath = Chr(34) & strDesktop & "\TestRhino6Automation.3dm" & Chr(34)

  ' Save the file  
  Call objRhino.RunScript("_-Save " & strPath, 0)
  Call WScript.Echo("File saved.")
 
  ' Exit Rhino
  Call objRhino.RunScript("_-Exit", 0)
  Call WScript.Echo("Done!")
   
End Sub

' Run the subroutine defined above
Call TestRhino6Automation
