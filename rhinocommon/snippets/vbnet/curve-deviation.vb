Partial Friend Class Examples
  Public Shared Function CrvDeviation(ByVal doc As RhinoDoc) As Result
	doc.Objects.UnselectAll()

	Dim obj_ref1 As ObjRef = Nothing
	Dim rc1 = RhinoGet.GetOneObject("first curve", True, ObjectType.Curve, obj_ref1)
	If rc1 IsNot Result.Success Then
	  Return rc1
	End If
	Dim curve_a As Curve = Nothing
	If obj_ref1 IsNot Nothing Then
	  curve_a = obj_ref1.Curve()
	End If
	If curve_a Is Nothing Then
	  Return Result.Failure
	End If

	' Since you already selected a curve if you don't unselect it
	' the next GetOneObject won't stop as it considers that curve
	' input, i.e., curveA and curveB will point to the same curve.
	' Another option would be to use an instance of Rhino.Input.Custom.GetObject
	' instead of Rhino.Input.RhinoGet as GetObject has a DisablePreSelect() method.
	doc.Objects.UnselectAll()

	Dim obj_ref2 As ObjRef = Nothing
	Dim rc2 = RhinoGet.GetOneObject("second curve", True, ObjectType.Curve, obj_ref2)
	If rc2 IsNot Result.Success Then
	  Return rc2
	End If
	Dim curve_b As Curve = Nothing
	If obj_ref2 IsNot Nothing Then
	  curve_b = obj_ref2.Curve()
	End If
	If curve_b Is Nothing Then
	  Return Result.Failure
	End If

	Dim tolerance = doc.ModelAbsoluteTolerance

	Dim max_distance As Double = Nothing
	Dim max_distance_parameter_a As Double = Nothing
	Dim max_distance_parameter_b As Double = Nothing
	Dim min_distance As Double = Nothing
	Dim min_distance_parameter_a As Double = Nothing
	Dim min_distance_parameter_b As Double = Nothing

	Dim conduit As DeviationConduit
	If Not Curve.GetDistancesBetweenCurves(curve_a, curve_b, tolerance, max_distance, max_distance_parameter_a, max_distance_parameter_b, min_distance, min_distance_parameter_a, min_distance_parameter_b) Then
	  RhinoApp.WriteLine("Unable to find overlap intervals.")
	  Return Result.Success
	Else
	  If min_distance <= RhinoMath.ZeroTolerance Then
		min_distance = 0.0
	  End If
	  Dim max_dist_pt_a = curve_a.PointAt(max_distance_parameter_a)
	  Dim max_dist_pt_b = curve_b.PointAt(max_distance_parameter_b)
	  Dim min_dist_pt_a = curve_a.PointAt(min_distance_parameter_a)
	  Dim min_dist_pt_b = curve_b.PointAt(min_distance_parameter_b)

	  conduit = New DeviationConduit(curve_a, curve_b, min_dist_pt_a, min_dist_pt_b, max_dist_pt_a, max_dist_pt_b) With {.Enabled = True}
	  doc.Views.Redraw()

	  RhinoApp.WriteLine("Minimum deviation = {0}   pointA({1}), pointB({2})", min_distance, min_dist_pt_a, min_dist_pt_b)
	  RhinoApp.WriteLine("Maximum deviation = {0}   pointA({1}), pointB({2})", max_distance, max_dist_pt_a, max_dist_pt_b)
	End If

	Dim str = ""
	RhinoGet.GetString("Press Enter when done", True, str)
	conduit.Enabled = False

	Return Result.Success
  End Function
End Class
