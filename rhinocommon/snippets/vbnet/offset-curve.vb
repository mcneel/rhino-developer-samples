Partial Friend Class Examples
  Public Shared Function OffsetCurve(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rs = RhinoGet.GetOneObject("Select Curve", False, ObjectType.Curve, obj_ref)
	If rs IsNot Result.Success Then
	  Return rs
	End If
	Dim curve = obj_ref.Curve()
	If curve Is Nothing Then
	  Return Result.Nothing
	End If

	Dim point As Point3d = Nothing
	rs = RhinoGet.GetPoint("Select Side", False, point)
	If rs IsNot Result.Success Then
	  Return rs
	End If
	If point Is Point3d.Unset Then
	  Return Result.Nothing
	End If

	Dim curves = curve.Offset(point, Vector3d.ZAxis, 1.0, doc.ModelAbsoluteTolerance, CurveOffsetCornerStyle.None)

	For Each offset_curve In curves
	  doc.Objects.AddCurve(offset_curve)
	Next offset_curve

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
