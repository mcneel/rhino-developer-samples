Imports System.Windows.Forms
Imports Rhino
Imports Rhino.Commands

Namespace examples_vb
  Public Class CaptureViewToBitmapCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbCaptureViewToBitmap"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim file_name = ""

      Dim bitmap = doc.Views.ActiveView.CaptureToBitmap(True, True, True)

      ' copy bitmap to clipboard
      Clipboard.SetImage(bitmap)

      ' save bitmap to file
      Dim save_file_dialog = New Rhino.UI.SaveFileDialog()
      save_file_dialog.Filter = "*.bmp"
      save_file_dialog.InitialDirectory =
        Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)

      If save_file_dialog.ShowDialog() = DialogResult.OK Then
        file_name = save_file_dialog.FileName
      End If

      If file_name <> "" Then
        bitmap.Save(file_name)
      End If

      Return Rhino.Commands.Result.Success
    End Function
  End Class
End Namespace