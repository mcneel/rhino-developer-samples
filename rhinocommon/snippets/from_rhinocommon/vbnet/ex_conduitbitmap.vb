Imports System.Drawing
Imports Rhino
Imports Rhino.Commands
Imports Rhino.Display

Namespace examples_vb
  Public Class DrawBitmapConduit
    Inherits Rhino.Display.DisplayConduit
    Private ReadOnly m_display_bitmap As DisplayBitmap

    Public Sub New()
      Dim flag = New System.Drawing.Bitmap(100, 100)
      For x As Integer = 0 To flag.Height - 1
        For y As Integer = 0 To flag.Width - 1
          flag.SetPixel(x, y, Color.White)
        Next
      Next

      Dim g = Graphics.FromImage(flag)
      g.FillEllipse(Brushes.Blue, 25, 25, 50, 50)
      m_display_bitmap = New DisplayBitmap(flag)
    End Sub

    Protected Overrides Sub DrawForeground(e As Rhino.Display.DrawEventArgs)
      e.Display.DrawBitmap(m_display_bitmap, 50, 50, Color.White)
    End Sub
  End Class

  Public Class DrawBitmapCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDrawBitmap"
      End Get
    End Property

    ReadOnly m_conduit As New DrawBitmapConduit()

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      ' toggle conduit on/off
      m_conduit.Enabled = Not m_conduit.Enabled

      RhinoApp.WriteLine("Custom conduit enabled = {0}", m_conduit.Enabled)
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace