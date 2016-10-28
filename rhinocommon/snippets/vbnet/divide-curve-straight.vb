Partial Friend Class Examples
  Private Shared Sub NextintersectParamAndPoint(ByVal overlapCurves() As Curve, ByVal intersectPoints() As Point3d, ByVal curve As Curve, ByRef intersectParam As Double, ByRef intersectPoint As Point3d)
	Dim intersect_params_and_points = New Dictionary(Of Double, Point3d)()
	For Each point In intersectPoints
	  Dim curve_param As Double = Nothing
	  curve.ClosestPoint(point, curve_param)
	  intersect_params_and_points(curve_param) = point
	Next point
	For Each overlap_curve In overlapCurves
	  intersect_params_and_points(overlap_curve.Domain.Min) = overlap_curve.PointAt(overlap_curve.Domain.Min)
	  intersect_params_and_points(overlap_curve.Domain.Max) = overlap_curve.PointAt(overlap_curve.Domain.Max)
	Next overlap_curve
	Dim min_t = intersect_params_and_points.Keys.Min()
	intersectParam = min_t
	intersectPoint = intersect_params_and_points(intersectParam)
  End Sub

  Public Shared Function DivideCurveStraight(ByVal doc As RhinoDoc) As Result
	' user input
	Dim obj_refs() As ObjRef = Nothing
	Dim rc = RhinoGet.GetMultipleObjects("Select curve to divide", False, ObjectType.Curve Or ObjectType.EdgeFilter, obj_refs)
	If rc IsNot Result.Success OrElse obj_refs Is Nothing Then
	  Return rc
	End If

	Dim distance_between_divisions As Double = 5
	rc = RhinoGet.GetNumber("Distance between divisions", False, distance_between_divisions, 1.0, Double.MaxValue)
	If rc IsNot Result.Success Then
	  Return rc
	End If


	' generate the points
	Dim points = New List(Of Point3d)()
	For Each obj_ref In obj_refs
	  Dim curve = obj_ref.Curve()
	  If curve Is Nothing Then
		  Return Result.Failure
	  End If

	  Dim t0 = curve.Domain.Min
	  points.Add(curve.PointAt(t0))

	  Dim sphere_center = curve.PointAt(t0)
	  Dim t = t0
	  Dim rest_of_curve = curve
	  Do
		Dim sphere = New Sphere(sphere_center, distance_between_divisions)
		Dim overlap_curves() As Curve = Nothing
		Dim intersect_points() As Point3d = Nothing
		Dim b = Intersection.CurveBrep(rest_of_curve, sphere.ToBrep(), 0.0, overlap_curves, intersect_points)
		If Not b OrElse (overlap_curves.Length = 0 AndAlso intersect_points.Length = 0) Then
		  Exit Do
		End If
		Dim intersect_param As Double = Nothing
		Dim intersect_point As Point3d = Nothing
		NextintersectParamAndPoint(overlap_curves, intersect_points, rest_of_curve, intersect_param, intersect_point)
		points.Add(intersect_point)
		t = intersect_param
		sphere_center = intersect_point
		rest_of_curve = curve.Split(t)(1)
	  Loop
	Next obj_ref

	For Each point In points
	  doc.Objects.AddPoint(point)
	Next point

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
