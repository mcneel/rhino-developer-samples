Partial Friend Class Examples
  Public Shared Function LineBetweenCurves(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim go As New Rhino.Input.Custom.GetObject()
	go.SetCommandPrompt("Select two curves")
	go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
	go.GetMultiple(2, 2)
	If go.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return go.CommandResult()
	End If

	Dim objRef0 As Rhino.DocObjects.ObjRef = go.Object(0)
	Dim objRef1 As Rhino.DocObjects.ObjRef = go.Object(1)

	Dim t0 As Double = Rhino.RhinoMath.UnsetValue
	Dim t1 As Double = Rhino.RhinoMath.UnsetValue
	Dim curve0 As Rhino.Geometry.Curve = objRef0.CurveParameter(t0)
	Dim curve1 As Rhino.Geometry.Curve = objRef1.CurveParameter(t1)
	If Nothing Is curve0 OrElse Not Rhino.RhinoMath.IsValidDouble(t0) OrElse Nothing Is curve1 OrElse Not Rhino.RhinoMath.IsValidDouble(t1) Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim line As Rhino.Geometry.Line = Rhino.Geometry.Line.Unset
	Dim rc As Boolean = Rhino.Geometry.Line.TryCreateBetweenCurves(curve0, curve1, t0, t1, False, False, line)
	If rc Then
	  If Guid.Empty <> doc.Objects.AddLine(line) Then
		doc.Views.Redraw()
		Return Rhino.Commands.Result.Success
	  End If
	End If
	Return Rhino.Commands.Result.Failure
  End Function
End Class
