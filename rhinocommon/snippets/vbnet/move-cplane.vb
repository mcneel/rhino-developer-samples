Partial Friend Class Examples
  Public Shared Function MoveCPlane(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim view As Rhino.Display.RhinoView = doc.Views.ActiveView
	If view Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim cplane As Rhino.DocObjects.ConstructionPlane = view.ActiveViewport.GetConstructionPlane()
	Dim origin As Point3d = cplane.Plane.Origin

	Dim gp As New MoveCPlanePoint(cplane)
	gp.SetCommandPrompt("CPlane origin")
	gp.SetBasePoint(origin, True)
	gp.DrawLineFromPoint(origin, True)
	gp.Get()

	If gp.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return gp.CommandResult()
	End If

	Dim point As Point3d = gp.Point()
	Dim v As Vector3d = origin - point
	If v.IsTiny() Then
	  Return Rhino.Commands.Result.Nothing
	End If

	Dim pl As Plane = cplane.Plane
	pl.Origin = point
	cplane.Plane = pl
	view.ActiveViewport.SetConstructionPlane(cplane)
	view.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
