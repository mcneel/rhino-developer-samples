Partial Friend Class Examples
  Public Shared Function CurveBoundingBox(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	' Select a curve object
	Dim rhObject As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select curve", False, Rhino.DocObjects.ObjectType.Curve, rhObject)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	' Validate selection
	Dim curve = rhObject.Curve()
	If curve Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	' Get the active view's construction plane
	Dim view = doc.Views.ActiveView
	If view Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If
	Dim plane = view.ActiveViewport.ConstructionPlane()

	' Compute the tight bounding box of the curve in world coordinates
	Dim bbox = curve.GetBoundingBox(True)
	If Not bbox.IsValid Then
	  Return Rhino.Commands.Result.Failure
	End If

	' Print the min and max box coordinates in world coordinates
	Rhino.RhinoApp.WriteLine("World min: {0}", bbox.Min)
	Rhino.RhinoApp.WriteLine("World max: {0}", bbox.Max)

	' Compute the tight bounding box of the curve based on the
	' active view's construction plane
	bbox = curve.GetBoundingBox(plane)

	' Print the min and max box coordinates in cplane coordinates
	Rhino.RhinoApp.WriteLine("CPlane min: {0}", bbox.Min)
	Rhino.RhinoApp.WriteLine("CPlane max: {0}", bbox.Max)
	Return Rhino.Commands.Result.Success
  End Function
End Class
