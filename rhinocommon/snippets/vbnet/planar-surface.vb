Partial Friend Class Examples
  Public Shared Function PlanarSurface(ByVal doc As RhinoDoc) As Result
	Dim corners() As Point3d = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetRectangle(corners)
	If rc IsNot Result.Success Then
	  Return rc
	End If

	Dim plane = New Plane(corners(0), corners(1), corners(2))

	Dim plane_surface = New PlaneSurface(plane, New Interval(0, corners(0).DistanceTo(corners(1))), New Interval(0, corners(1).DistanceTo(corners(2))))

	doc.Objects.Add(plane_surface)
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
