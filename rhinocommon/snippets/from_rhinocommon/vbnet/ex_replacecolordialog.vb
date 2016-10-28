Imports Rhino
Imports Rhino.Commands
Imports Rhino.UI
Imports System.Windows.Forms

Namespace examples_vb
  Public Class ReplaceColorDialogCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbReplaceColorDialog"
      End Get
    End Property

    Private m_dlg As ColorDialog = Nothing

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dialogs.SetCustomColorDialog(AddressOf OnSetCustomColorDialog)
      Return Result.Success
    End Function

    Private Sub OnSetCustomColorDialog(sender As Object, e As GetColorEventArgs)

      m_dlg = New ColorDialog()
      If m_dlg.ShowDialog(Nothing) = DialogResult.OK Then
        Dim c = m_dlg.Color
        e.SelectedColor = c
      End If
    End Sub
  End Class
End Namespace