'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' RhinoRender.vbs -- October 2014
' If this code works, it was written by Dale Fugier.
' If not, I don't know who wrote it.
' Works with Rhinoceros 5.
'
' Run from a Windows command prompt with this syntax:
'
'   C:\> CSCRIPT RhinoRender.vbs FileName.3dm
'
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Option Explicit

' Variable declarations
Dim oRhino, oRhinoScript
Dim sFile, sBitmap
Dim oFSO, nRetries

' Create a FileSystemObject object
Set oFSO = CreateObject("Scripting.FileSystemObject")

' Get the file name as a command line argument  
If WScript.Arguments.Count = 1 Then
  sFile = oFSO.GetAbsolutePathName(WScript.Arguments.Item(0))
Else
  Call WScript.Echo("Usage: RhinoRender.vbs FileName.3dm")
  WScript.Quit
End If

' Verify the file exists  
If Not oFSO.FileExists(sFile) Then
  Call WScript.Echo("File does not exist!")
  WScript.Quit
End If

' Verity the file name is for a 3DM file
If (InStr(LCase(sFile), ".3dm") > 0) Then
  sBitmap = Replace(sFile, ".3dm", ".png", 1, -1, 1)
Else
  Call WScript.Echo("File is not a Rhino 3dm file.")
  WScript.Quit
End If

' Try creating a Rhino 5 64-bit application object
On Error Resume Next
Set oRhino = CreateObject("Rhino5x64.Application")
If Err.Number <> 0 Then
  On Error Resume Next
  ' Try creating a Rhino 5 32-bit application object
  Set objRhino = CreateObject("Rhino5.Application")
  If Err.Number <> 0 Then
    Call WScript.Echo("Failed to create Rhino object.")
    WScript.Quit
  End If
End If
Call WScript.Echo("Rhino created")  
  
' Wait for Rhino to finish initializing
nRetries = 0
Do While (nRetries < 10)
  Set oRhinoScript = oRhino.GetScriptObject
  If objRhino.IsInitialized = 0 Then
    Call WScript.Sleep(500)
    nRetries = nRetries + 1
  Else
    Call WScript.Echo("Rhino initialization failed")
    WScript.Quit     
  End If
Loop
Call WScript.Echo("Rhino initialized")  

' Try getting RhinoScript object
Set oRhinoScript = oRhino.GetScriptObject

' Start rendering process
Call WScript.Echo("Processing " & sFile)
oRhinoScript.DocumentModified = False

' Open the file
Call WScript.Echo("Opening")
Call oRhinoScript.Command("_-Open " & Chr(34) & sFile & Chr(34))

' Render
Call WScript.Echo("Rendering")
Call oRhinoScript.Command("_-Render")

' Save the rendered image
Call WScript.Echo("Saving " & sBitmap)
Call oRhinoScript.Command("_-SaveRenderWindowAs " & Chr(34) & sBitmap & Chr(34))

Call WScript.Echo("Cleaning up")

' Close the render window
Call oRhinoScript.Command("_-CloseRenderWindow")

' Clear the document modified so Rhino does not prompt us
' to save the model when closing.  
oRhinoScript.DocumentModified = False

' Done!
Call WScript.Echo("Done!")
