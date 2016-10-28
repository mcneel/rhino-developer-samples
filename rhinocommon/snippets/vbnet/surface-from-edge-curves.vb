Partial Friend Class Examples
  Public Shared Function EdgeSrf(ByVal doc As RhinoDoc) As Result
	Dim go = New GetObject()
	go.SetCommandPrompt("Select 2, 3, or 4 open curves")
	go.GeometryFilter = ObjectType.Curve
	go.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
	go.GetMultiple(2, 4)
	If go.CommandResult() <> Result.Success Then
	  Return go.CommandResult()
	End If

	Dim curves = go.Objects().Select(Function(o) o.Curve())

	Dim brep = Brep.CreateEdgeSurface(curves)

	If brep IsNot Nothing Then
	  doc.Objects.AddBrep(brep)
	  doc.Views.Redraw()
	End If

	Return Result.Success
  End Function
End Class
