Partial Friend Class Examples
  Public Shared Function AddNamedView(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim view As Rhino.Display.RhinoView = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetView("Select view to adjust", view)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim location As Rhino.Geometry.Point3d = Nothing
	rc = Rhino.Input.RhinoGet.GetPoint("Camera Location", False, location)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim gp As New Rhino.Input.Custom.GetPoint()
	gp.SetCommandPrompt("Look At Location")
	gp.DrawLineFromPoint(location, False)
	gp.Get()
	If gp.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim lookat As Rhino.Geometry.Point3d = gp.Point()

	Dim name As String = view.ActiveViewport.Name
	rc = Rhino.Input.RhinoGet.GetString("Name", True, name)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim vp As Rhino.Display.RhinoViewport = view.ActiveViewport
	' save the current viewport projection
	vp.PushViewProjection()
	vp.CameraUp = Rhino.Geometry.Vector3d.ZAxis
	vp.SetCameraLocation(location, False)
	vp.SetCameraDirection(lookat - location, True)
	vp.Name = name

	doc.NamedViews.Add(name, vp.Id)
	view.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
