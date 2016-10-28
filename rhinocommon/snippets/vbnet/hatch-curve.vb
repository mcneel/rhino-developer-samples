Partial Friend Class Examples
  Public Shared Function HatchCurve(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim go = New Rhino.Input.Custom.GetObject()
	go.SetCommandPrompt("Select closed planar curve")
	go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
	go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve
	go.SubObjectSelect = False
	go.Get()
	If go.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return go.CommandResult()
	End If

	Dim curve = go.Object(0).Curve()
	If curve Is Nothing OrElse Not curve.IsClosed OrElse Not curve.IsPlanar() Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim hatch_name As String = doc.HatchPatterns(doc.HatchPatterns.CurrentHatchPatternIndex).Name
	Dim rc = Rhino.Input.RhinoGet.GetString("Hatch pattern", True, hatch_name)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If
	hatch_name = hatch_name.Trim()
	If String.IsNullOrWhiteSpace(hatch_name) Then
	  Return Rhino.Commands.Result.Nothing
	End If
	Dim index As Integer = doc.HatchPatterns.Find(hatch_name, True)
	If index < 0 Then
	  Rhino.RhinoApp.WriteLine("Hatch pattern does not exist.")
	  Return Rhino.Commands.Result.Nothing
	End If

	Dim hatches = Rhino.Geometry.Hatch.Create(curve, index, 0, 1)
	For i As Integer = 0 To hatches.Length - 1
	  doc.Objects.AddHatch(hatches(i))
	Next i
	If hatches.Length>0 Then
	  doc.Views.Redraw()
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
