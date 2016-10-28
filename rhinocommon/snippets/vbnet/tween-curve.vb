Partial Friend Class Examples
  Public Shared Function TweenCurve(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim go As New Rhino.Input.Custom.GetObject()
	go.SetCommandPrompt("Select two curves")
	go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
	go.GetMultiple(2, 2)
	If go.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return go.CommandResult()
	End If

	Dim curve0 As Rhino.Geometry.Curve = go.Object(0).Curve()
	Dim curve1 As Rhino.Geometry.Curve = go.Object(1).Curve()
	If Nothing IsNot curve0 AndAlso Nothing IsNot curve1 Then
	  Dim curves() As Rhino.Geometry.Curve = Rhino.Geometry.Curve.CreateTweenCurves(curve0, curve1, 1)
	  If Nothing IsNot curves Then
		For i As Integer = 0 To curves.Length - 1
		  doc.Objects.AddCurve(curves(i))
		Next i

		doc.Views.Redraw()
		Return Rhino.Commands.Result.Success
	  End If
	End If

	Return Rhino.Commands.Result.Failure
  End Function
End Class
