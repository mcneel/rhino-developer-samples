Imports Rhino
Imports Rhino.Commands

Namespace examples_vb
  Public Class ViewportResolutionCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbViewportResolution"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim activeViewport = doc.Views.ActiveView.ActiveViewport
      RhinoApp.WriteLine([String].Format("Name = {0}: Width = {1}, Height = {2}", activeViewport.Name, activeViewport.Size.Width, activeViewport.Size.Height))
      Return Result.Success
    End Function
  End Class
End Namespace