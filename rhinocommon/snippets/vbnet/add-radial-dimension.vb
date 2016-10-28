Partial Friend Class Examples
  Public Shared Function AddRadialDimension(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select curve for radius dimension", True, ObjectType.Curve, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim curve_parameter As Double = Nothing
	Dim curve = obj_ref.CurveParameter(curve_parameter)
	If curve Is Nothing Then
	  Return Result.Failure
	End If

	If curve.IsLinear() OrElse curve.IsPolyline() Then
	  RhinoApp.WriteLine("Curve must be non-linear.")
	  Return Result.Nothing
	End If

	' in this example just deal with planar curves
	If Not curve.IsPlanar() Then
	  RhinoApp.WriteLine("Curve must be planar.")
	  Return Result.Nothing
	End If

	Dim point_on_curve = curve.PointAt(curve_parameter)
	Dim curvature_vector = curve.CurvatureAt(curve_parameter)
	Dim len = curvature_vector.Length
	If len < RhinoMath.SqrtEpsilon Then
	  RhinoApp.WriteLine("Curve is almost linear and therefore has no curvature.")
	  Return Result.Nothing
	End If

	Dim center = point_on_curve + (curvature_vector/(len*len))
	Dim plane As Plane = Nothing
	curve.TryGetPlane(plane)
	Dim radial_dimension = New RadialDimension(center, point_on_curve, plane.XAxis, plane.Normal, 5.0)
	doc.Objects.AddRadialDimension(radial_dimension)
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
