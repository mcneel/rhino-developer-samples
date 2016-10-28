Partial Friend Class Examples
  Public Shared Function FindCurveParameterAtPoint(ByVal doc As RhinoDoc) As Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select curve", True, ObjectType.Curve,objref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim curve = objref.Curve()
	If curve Is Nothing Then
	  Return Result.Failure
	End If

	Dim gp = New GetPoint()
	gp.SetCommandPrompt("Pick a location on the curve")
	gp.Constrain(curve, False)
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If

	Dim point = gp.Point()
	Dim closest_point_param As Double = Nothing
	If curve.ClosestPoint(point, closest_point_param) Then
	  RhinoApp.WriteLine("point: ({0}), parameter: {1}", point, closest_point_param)
	  doc.Objects.AddPoint(point)
	  doc.Views.Redraw()
	End If
	Return Result.Success
  End Function
End Class
