Partial Friend Class Examples
  Public Shared Function CircleCenter(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim go As New Rhino.Input.Custom.GetObject()
	go.SetCommandPrompt("Select objects")
	go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
	go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve
	go.GetMultiple(1, 0)
	If go.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return go.CommandResult()
	End If

	Dim objrefs() As Rhino.DocObjects.ObjRef = go.Objects()
	If objrefs Is Nothing Then
	  Return Rhino.Commands.Result.Nothing
	End If

	Dim tolerance As Double = doc.ModelAbsoluteTolerance
	For i As Integer = 0 To objrefs.Length - 1
	  ' get the curve geometry
	  Dim curve As Rhino.Geometry.Curve = objrefs(i).Curve()
	  If curve Is Nothing Then
		Continue For
	  End If
	  Dim circle As Rhino.Geometry.Circle = Nothing
	  If curve.TryGetCircle(circle, tolerance) Then
		Rhino.RhinoApp.WriteLine("Circle{0}: center = {1}", i+1, circle.Center)
	  End If
	Next i
	Return Rhino.Commands.Result.Success
  End Function
End Class
