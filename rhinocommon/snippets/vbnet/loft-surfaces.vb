Partial Friend Class Examples
  Public Shared Function Loft(ByVal doc As RhinoDoc) As Result
	' select curves to loft
	Dim gs = New GetObject()
	gs.SetCommandPrompt("select curves to loft")
	gs.GeometryFilter = ObjectType.Curve
	gs.DisablePreSelect()
	gs.SubObjectSelect = False
	gs.GetMultiple(2, 0)
	If gs.CommandResult() <> Result.Success Then
	  Return gs.CommandResult()
	End If

	Dim curves = gs.Objects().Select(Function(obj) obj.Curve()).ToList()

	Dim breps = Brep.CreateFromLoft(curves, Point3d.Unset, Point3d.Unset, LoftType.Tight, False)
	For Each brep In breps
	  doc.Objects.AddBrep(brep)
	Next brep

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
