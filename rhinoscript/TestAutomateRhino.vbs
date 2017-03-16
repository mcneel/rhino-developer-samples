'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' TestAutomateRhino.rvb -- May 2013, Dale Fugier.
' If this code works, it was written by Dale Fugier.
' If not, I don't know who wrote it.
' Works with Rhino 4.0.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Option Explicit

Sub TestAutomateRhino

  Dim objRhino, objRhinoScript, objShell, arrPoints
  Dim strDesktop, strPath
  
  ' Create the Rhino application object
  On Error Resume Next
  Set objRhino = CreateObject("Rhino4.Application")
  If Err.Number <> 0 Then
      Call WScript.Echo("Failed to create Rhino object.")
      Exit Sub
  End If
  Call WScript.Echo("Rhino object created.")

  ' Since the RhinoScript object resides in a plug-in, it is possible
  ' to try to get the RhinoScript object before it is loaded. So
  ' sometimes we might need to make a few attempts...
  Do While (nCount < 10)
    On Error Resume Next
    Set objRhinoScript = objRhino.GetScriptObject()
    If Err.Number <> 0 Then
      Err.Clear()
      Call WScrpt.Sleep(500)
      nCount = nCount + 1
    Else
      Exit Do
    End If
  Loop

  If objRhinoScript Is Nothing Then
    Call WScript.Echo("Failed to create RhinoScript object.")
    Exit Sub
  End If
  Call WScript.Echo("RhinoScript object created.")
  
  ' Add a bow-tie polyline to the document
  arrPoints = Array( _
    Array(-10, -5, 0), _
    Array(10, 5, 0), _
    Array(-10, 5, 0), _
    Array(10, -5, 0), _
    Array(-10,-5,0) _
    )
  
  Call objRhinoScript.AddPolyline(arrPoints)
  
  ' Create a shell object
  Set objShell = WScript.CreateObject("WScript.Shell")
  Call WScript.Echo("Shell object created.")
  
  ' Get the desktop folder
  strDesktop = objShell.SpecialFolders("Desktop")
  strPath = Chr(34) & strDesktop & "\TestAutomateRhino.3dm" & Chr(34)

  ' Save the file  
  Call objRhinoScript.Command("_-Save " & strPath, 0)
  
  ' Set the document's modified flag to false so we are not
  ' prompted to save.
  Call objRhinoScrpt.DocumentModified(False)
  
  ' Exit Rhino
  Call objRhinoScript.Command("_-Exit", 0)
   
End Sub

' Run the subroutine defined above
Call TestAutomateRhino
  