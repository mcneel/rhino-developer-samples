Partial Friend Class Examples
  Public Shared Function ExtrudeBrepFace(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim go0 As New Rhino.Input.Custom.GetObject()
	go0.SetCommandPrompt("Select surface to extrude")
	go0.GeometryFilter = Rhino.DocObjects.ObjectType.Surface
	go0.SubObjectSelect = True
	go0.Get()
	If go0.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return go0.CommandResult()
	End If

	Dim face As Rhino.Geometry.BrepFace = go0.Object(0).Face()
	If Nothing Is face Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim go1 As New Rhino.Input.Custom.GetObject()
	go1.SetCommandPrompt("Select path curve")
	go1.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
	go1.SubObjectSelect = True
	go1.DeselectAllBeforePostSelect = False
	go1.Get()
	If go1.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return go1.CommandResult()
	End If

	Dim curve As Rhino.Geometry.Curve = go1.Object(0).Curve()
	If Nothing Is curve Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim brep As Rhino.Geometry.Brep = face.CreateExtrusion(curve, True)
	If Nothing IsNot brep Then
	  doc.Objects.Add(brep)
	  doc.Views.Redraw()
	End If

	Return Rhino.Commands.Result.Success
  End Function
End Class
