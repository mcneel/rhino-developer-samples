Partial Friend Class Examples
  Public Shared Function Leader(ByVal doc As RhinoDoc) As Result
	Dim points = New Point3d() {
		New Point3d(1, 1, 0),
		New Point3d(5, 1, 0),
		New Point3d(5, 5, 0),
		New Point3d(9, 5, 0)
	}

	Dim xy_plane = Plane.WorldXY

	Dim points2d = New List(Of Point2d)()
	For Each point3d In points
	  Dim x As Double = Nothing, y As Double = Nothing
	  If xy_plane.ClosestParameter(point3d, x, y) Then
		Dim point2d = New Point2d(x, y)
		If points2d.Count < 1 OrElse point2d.DistanceTo(points2d.Last()) > RhinoMath.SqrtEpsilon Then
		  points2d.Add(point2d)
		End If
	  End If
	Next point3d

	doc.Objects.AddLeader(xy_plane, points2d)
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
