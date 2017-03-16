'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' AddStartupScripts.rvb -- July 2016
' If this code works, it was written by Dale Fugier.
' If not, I don't know who wrote it.
' Works with Rhino 5.
'
' Run from a Windows command prompt with this syntax:
'
'   C:\> CSCRIPT AddStartupScripts.rvb
'
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Option Explicit

' Run the subroutine below...
Call AddStartupScripts

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Adds some script file to RhinoScript's list of script files to load
' when Rhino starts.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Sub AddStartupScripts
	
	Call AddStartupScriptFile("C:\Users\Dale\Desktop\Test1.rvb")
	Call AddStartupScriptFile("C:\Users\Dale\Desktop\Test2.rvb")
	Call AddStartupScriptFile("C:\Users\Dale\Desktop\Test3.rvb")

End Sub

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Adds a script file to RhinoScript's list of script files to load
' when Rhino starts.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Sub AddStartupScriptFile(strFile)
	
  ' Declare local constants
	Const KEY0 = "HKCU\SOFTWARE\McNeel\Rhinoceros\5.0x64\Scheme: Default\Plug-ins\1c7a3523-9a8f-4cec-a8e0-310f580536a7\Settings\StartupFileListCount"
	Const KEY1 = "HKCU\SOFTWARE\McNeel\Rhinoceros\5.0x64\Scheme: Default\Plug-ins\1c7a3523-9a8f-4cec-a8e0-310f580536a7\Settings\StartupFileList\File"
	
  ' Declare local variables
	Dim objFSO, objShell, strKey, strValue, nCount

  ' Create a Scripting.FileSystemObject object
	Set objFSO = CreateObject("Scripting.FileSystemObject")
  
  ' Verify the script file exits
	If Not objFSO.FileExists(strFile) Then
		Call WScript.Echo("File not found.")
		Exit Sub
	End If
	
  ' Create a WScript.Shell object
	Set objShell = CreateObject("WScript.Shell")
	
  ' Read the 'StartupFileListCount' string from the Registry
	strValue = objShell.RegRead(KEY0)
	If IsNull(strValue) Then
		Call WScript.Echo("StartupFileListCount value not found.")
		Exit Sub
	End If

  ' Convert the string to an integer
	nCount = CInt(strValue)
  
  ' Increment the integer and write it to the Registry as a string
	Call objShell.RegWrite(KEY0, CStr(nCount + 1), "REG_SZ")
	
  ' Create a key that reads "FileX", where X = count
	strKey = KEY1 & CStr(nCount)
  
  ' Write the script file to the Registry
	Call objShell.RegWrite(strKey, strFile, "REG_SZ")
	
End Sub