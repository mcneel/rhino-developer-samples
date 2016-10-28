Partial Friend Class Examples
  Public Shared Function Sweep1(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim rail_ref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select rail curve", False, Rhino.DocObjects.ObjectType.Curve, rail_ref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim rail_crv = rail_ref.Curve()
	If rail_crv Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim gx = New Rhino.Input.Custom.GetObject()
	gx.SetCommandPrompt("Select cross section curves")
	gx.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
	gx.EnablePreSelect(False, True)
	gx.GetMultiple(1,0)
	If gx.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return gx.CommandResult()
	End If

	Dim cross_sections = New List(Of Rhino.Geometry.Curve)()
	For i As Integer = 0 To gx.ObjectCount - 1
	  Dim crv = gx.Object(i).Curve()
	  If crv IsNot Nothing Then
		cross_sections.Add(crv)
	  End If
	Next i
	If cross_sections.Count<1 Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim sweep = New Rhino.Geometry.SweepOneRail()
	sweep.AngleToleranceRadians = doc.ModelAngleToleranceRadians
	sweep.ClosedSweep = False
	sweep.SweepTolerance = doc.ModelAbsoluteTolerance
	sweep.SetToRoadlikeTop()
	Dim breps = sweep.PerformSweep(rail_crv, cross_sections)
	For i As Integer = 0 To breps.Length - 1
	  doc.Objects.AddBrep(breps(i))
	Next i
	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
