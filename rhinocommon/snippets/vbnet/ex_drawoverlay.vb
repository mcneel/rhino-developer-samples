Imports Rhino

<System.Runtime.InteropServices.Guid("318269a3-aff8-4b97-ac7d-bef2d8a3338f")> _
Public Class ex_drawoverlay
  Inherits Rhino.Commands.Command

  Public Overrides ReadOnly Property EnglishName() As String
    Get
      Return "ex_drawoverlay"
    End Get
  End Property

  ReadOnly m_conduit As New CustomConduit()

  Protected Overrides Function RunCommand(ByVal doc As RhinoDoc, ByVal mode As Rhino.Commands.RunMode) As Rhino.Commands.Result
    ' toggle conduit on/off
    m_conduit.Enabled = Not m_conduit.Enabled

    RhinoApp.WriteLine("Custom conduit enabled = {0}", m_conduit.Enabled)
    doc.Views.Redraw()
    Return Rhino.Commands.Result.Success
  End Function
End Class


Class CustomConduit
  Inherits Rhino.Display.DisplayConduit
  Protected Overrides Sub DrawForeground(e As Rhino.Display.DrawEventArgs)
    Dim bounds = e.Viewport.Bounds
    Dim pt = New Rhino.Geometry.Point2d(bounds.Right - 100, bounds.Bottom - 30)
    e.Display.Draw2dText("Hello", System.Drawing.Color.Red, pt, False)
  End Sub
End Class
