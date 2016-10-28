Partial Friend Class Examples
  Public Shared Function FilletCurves(ByVal doc As RhinoDoc) As Result
	Dim gc1 = New GetObject()
	gc1.DisablePreSelect()
	gc1.SetCommandPrompt("Select first curve to fillet (close to the end you want to fillet)")
	gc1.GeometryFilter = ObjectType.Curve
	gc1.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
	gc1.Get()
	If gc1.CommandResult() <> Result.Success Then
	  Return gc1.CommandResult()
	End If
	Dim curve1_obj_ref = gc1.Object(0)
	Dim curve1 = curve1_obj_ref.Curve()
	If curve1 Is Nothing Then
		Return Result.Failure
	End If
	Dim curve1_point_near_end = curve1_obj_ref.SelectionPoint()
	If curve1_point_near_end Is Point3d.Unset Then
	  Return Result.Failure
	End If

	Dim gc2 = New GetObject()
	gc2.DisablePreSelect()
	gc2.SetCommandPrompt("Select second curve to fillet (close to the end you want to fillet)")
	gc2.GeometryFilter = ObjectType.Curve
	gc2.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
	gc2.Get()
	If gc2.CommandResult() <> Result.Success Then
	  Return gc2.CommandResult()
	End If
	Dim curve2_obj_ref = gc2.Object(0)
	Dim curve2 = curve2_obj_ref.Curve()
	If curve2 Is Nothing Then
		Return Result.Failure
	End If
	Dim curve2_point_near_end = curve2_obj_ref.SelectionPoint()
	If curve2_point_near_end Is Point3d.Unset Then
	  Return Result.Failure
	End If

	Dim radius As Double = 0
	Dim rc = RhinoGet.GetNumber("fillet radius", False, radius)
	If rc IsNot Result.Success Then
		Return rc
	End If

	Dim join = False
	Dim trim = True
	Dim arc_extension = True
	Dim fillet_curves = Curve.CreateFilletCurves(curve1, curve1_point_near_end, curve2, curve2_point_near_end, radius, join, trim, arc_extension, doc.ModelAbsoluteTolerance, doc.ModelAngleToleranceDegrees)
	If fillet_curves Is Nothing Then '|| fillet_curves.Length != 3
	  Return Result.Failure
	End If

	For Each fillet_curve In fillet_curves
	  doc.Objects.AddCurve(fillet_curve)
	Next fillet_curve
	doc.Views.Redraw()
	Return rc
  End Function
End Class
