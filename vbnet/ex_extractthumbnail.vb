Imports Rhino
Imports Rhino.Commands
Imports Rhino.Input
Imports Rhino.Input.Custom
Imports System.Windows
Imports System.Windows.Controls

Namespace examples_vb
  Public Class ExtractThumbnailCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbExtractThumbnail"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim gf = RhinoGet.GetFileName(GetFileNameMode.OpenImage, "*.3dm", "select file", Nothing)
      If gf = String.Empty OrElse Not System.IO.File.Exists(gf) Then
        Return Result.Cancel
      End If

      Dim bitmap = Rhino.FileIO.File3dm.ReadPreviewImage(gf)
      ' convert System.Drawing.Bitmap to BitmapSource
      Dim imageSource = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(bitmap.GetHbitmap(), IntPtr.Zero, Int32Rect.Empty, System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions())

      ' show in WPF window
      Dim window = New Window()
      Dim image = New Image()
      image.Source = imageSource

      window.Content = image
      window.Show()

      Return Result.Success
    End Function
  End Class
End Namespace