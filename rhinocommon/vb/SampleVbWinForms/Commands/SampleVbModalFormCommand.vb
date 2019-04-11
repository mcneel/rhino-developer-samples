Imports System.Windows.Forms
Imports Rhino
Imports Rhino.Commands
Imports RhinoWindows

Namespace SampleVbWinForms

  Public Class SampleVbModalFormCommand
    Inherits Command

    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "SampleVbModalForm"
      End Get
    End Property

    Protected Overrides Function RunCommand(ByVal doc As RhinoDoc, ByVal mode As RunMode) As Result

      Dim rc As Result = Commands.Result.Cancel

      If mode = RunMode.Interactive Then
        Dim form = New SampleVbModalForm With {.StartPosition = FormStartPosition.CenterParent}
        Dim result As DialogResult = form.ShowDialog(RhinoWinApp.MainWindow)
        If result = DialogResult.OK Then
          rc = Commands.Result.Success
        End If
      Else
        Dim msg = String.Format("Scriptable version of {0} command not implemented.", EnglishName)
        RhinoApp.WriteLine(msg)
      End If

      Return rc

    End Function
  End Class
End Namespace