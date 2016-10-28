Imports System.Collections.Generic
Imports Rhino
Imports Rhino.Commands
Imports Rhino.Geometry

Namespace examples_vb
  Public Class SortPointsCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbSortPoints"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim points = New List(Of Point3d)() From { _
        New Point3d(0, 0, 0), _
        New Point3d(0, 0, 1), _
        New Point3d(0, 1, 0), _
        New Point3d(0, 1, 1), _
        New Point3d(1, 0, 0), _
        New Point3d(1, 0, 1), _
        New Point3d(1, 1, 0), _
        New Point3d(1, 1, 1) _
      }

      RhinoApp.WriteLine("Before sort ...")
      For Each point As Point3d In points
        RhinoApp.WriteLine("point: {0}", point)
      Next

      Dim sorted_points = Point3d.SortAndCullPointList(points, doc.ModelAbsoluteTolerance)

      RhinoApp.WriteLine("After sort ...")
      For Each point As Point3d In sorted_points
        RhinoApp.WriteLine("point: {0}", point)
      Next

      doc.Objects.AddPoints(sorted_points)
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace