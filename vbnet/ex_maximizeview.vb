Imports System.Runtime.InteropServices
Imports Rhino
Imports Rhino.Commands

Namespace examples_vb
  Public Class MaximizeViewCommand
    Inherits Command
    <DllImport("user32.dll", ExactSpelling:=True, CharSet:=CharSet.Auto)> _
    Public Shared Function GetParent(hWnd As IntPtr) As IntPtr
    End Function

    <DllImport("user32.dll")> _
    Private Shared Function ShowWindow(hWnd As IntPtr, nCmdShow As Integer) As Boolean
    End Function

    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbMaximizeView"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim parent_handle As IntPtr = GetParent(doc.Views.ActiveView.Handle)
      If parent_handle <> IntPtr.Zero Then
        ShowWindow(parent_handle, 3)
      End If
      Return Result.Success
    End Function
  End Class
End Namespace