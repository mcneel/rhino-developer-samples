Imports Rhino
Imports Rhino.Geometry
Imports Rhino.Commands
Imports System.Collections.Generic
Imports System.Linq

Namespace examples_vb
  Public Class LeaderCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbLeader"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim points = New List(Of Point3d)() From { _
        New Point3d(1, 1, 0), _
        New Point3d(5, 1, 0), _
        New Point3d(5, 5, 0), _
        New Point3d(9, 5, 0) _
      }

      Dim xyPlane = Plane.WorldXY

      Dim points2d = New List(Of Point2d)()
      For Each point3d As Point3d In points
        Dim x As Double, y As Double
        If xyPlane.ClosestParameter(point3d, x, y) Then
          Dim point2d = New Point2d(x, y)
          If points2d.Count < 1 OrElse point2d.DistanceTo(points2d.Last()) > RhinoMath.SqrtEpsilon Then
            points2d.Add(point2d)
          End If
        End If
      Next

      doc.Objects.AddLeader(xyPlane, points2d)
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace