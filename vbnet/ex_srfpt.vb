Imports Rhino
Imports Rhino.Geometry
Imports Rhino.Commands

Namespace examples_vb
  Public Class SurfaceFromCornersCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbSurfaceFromCorners"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim surface = NurbsSurface.CreateFromCorners(
        New Point3d(5, 0, 0),
        New Point3d(5, 5, 5),
        New Point3d(0, 5, 0),
        New Point3d(0, 0, 0))

      doc.Objects.AddSurface(surface)
      doc.Views.Redraw()

      Return Rhino.Commands.Result.Success
    End Function
  End Class
End Namespace