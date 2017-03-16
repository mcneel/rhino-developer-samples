Option Explicit

Call BatchRender

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' BatchRender
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Sub BatchRender
  
  Dim oRhino, oRhinoScript, nCount
  Dim oFSO, oFolder, oFile 
  Dim sFolder, sFile, sBitmap
  
  sFolder = "C:\Test"

  WScript.Echo "Creating Rhino object..."
  On Error Resume Next
  Set oRhino = CreateObject("Rhino4.Application")
  If (Err.Number <> 0) Then
    WScript.Echo "Failed to create Rhino object."
    Exit Sub
  Else
    WScript.Echo "Rhino object created."
  End If
  
  WScript.Echo "Retrieving RhinoScript object..."
  nCount = 0
  Do While (nCount < 10)
   On Error Resume Next
   Set oRhinoScript = oRhino.GetScriptObject
   If Err.Number <> 0 Then
     Err.Clear
     WScript.Sleep 500
     nCount = nCount + 1
   Else
     Exit Do
   End If
  Loop

  If (oRhinoScript Is Nothing) Then
    WScript.Echo "Failed to retrieve RhinoScript object."
    Set oRhino = Nothing
    Exit Sub
  Else
    WScript.Echo "RhinoScript object retrieved."
  End If

  WScript.Echo "Creating file system object."
  Set oFSO = CreateObject("Scripting.FileSystemObject")
  Set oFolder = oFSO.GetFolder(sFolder)
  For Each oFile In oFolder.Files
    sFile = oFile.Path
    If (InStr(LCase(sFile), ".3dm") > 0) Then
      sBitmap = Replace(sFile, ".3dm", ".jpg", 1, -1, 1)
      WScript.Echo "Processing " & sFile & "..."
      oRhinoScript.DocumentModified = False
      WScript.Echo "  Opening"
      oRhinoScript.Command "_-Open " & Chr(34) & sFile & Chr(34)
      WScript.Echo "  Rendering"
      oRhinoScript.Command "_-Render"
      WScript.Echo "  Saving"
      oRhinoScript.Command "_-SaveRenderWindowAs " & Chr(34) & sBitmap & Chr(34)
      WScript.Echo "  Closing render window"
      oRhinoScript.Command "_-CloseRenderWindow"
      oRhinoScript.DocumentModified = False
    End If  
  Next

  WScript.Echo "Cleaning up..."
  Set oFile = Nothing
  Set oFolder = Nothing
  Set oFSO = Nothing
  Set oRhinoScript = Nothing
  Set oRhino = Nothing
  WScript.Echo "Done!"

End Sub
