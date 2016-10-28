Partial Friend Class Examples
  Public Shared Function CurveSurfaceIntersect(ByVal doc As RhinoDoc) As Result
	Dim gs = New GetObject()
	gs.SetCommandPrompt("select brep")
	gs.GeometryFilter = ObjectType.Brep
	gs.DisablePreSelect()
	gs.SubObjectSelect = False
	gs.Get()
	If gs.CommandResult() <> Result.Success Then
	  Return gs.CommandResult()
	End If
	Dim brep = gs.Object(0).Brep()

	Dim gc = New GetObject()
	gc.SetCommandPrompt("select curve")
	gc.GeometryFilter = ObjectType.Curve
	gc.DisablePreSelect()
	gc.SubObjectSelect = False
	gc.Get()
	If gc.CommandResult() <> Result.Success Then
	  Return gc.CommandResult()
	End If
	Dim curve = gc.Object(0).Curve()

	If brep Is Nothing OrElse curve Is Nothing Then
	  Return Result.Failure
	End If

	Dim tolerance = doc.ModelAbsoluteTolerance

	Dim intersection_points() As Point3d = Nothing
	Dim overlap_curves() As Curve = Nothing
	If Not Intersection.CurveBrep(curve, brep, tolerance, overlap_curves, intersection_points) Then
	  RhinoApp.WriteLine("curve brep intersection failed")
	  Return Result.Nothing
	End If

	For Each overlap_curve In overlap_curves
	  doc.Objects.AddCurve(overlap_curve)
	Next overlap_curve
	For Each intersection_point In intersection_points
	  doc.Objects.AddPoint(intersection_point)
	Next intersection_point

	RhinoApp.WriteLine("{0} overlap curves, and {1} intersection points", overlap_curves.Length, intersection_points.Length)
	doc.Views.Redraw()

	Return Result.Success
  End Function
End Class
