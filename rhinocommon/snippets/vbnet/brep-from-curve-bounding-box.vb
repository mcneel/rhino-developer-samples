Partial Friend Class Examples
  Public Shared Function BrepFromCurveBBox(ByVal doc As RhinoDoc) As Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select Curve", False, ObjectType.Curve, objref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim curve = objref.Curve()

	Dim view = doc.Views.ActiveView
	Dim plane = view.ActiveViewport.ConstructionPlane()
	' Create a construction plane aligned bounding box
	Dim bbox = curve.GetBoundingBox(plane)

	If bbox.IsDegenerate(doc.ModelAbsoluteTolerance) > 0 Then
	  RhinoApp.WriteLine("the curve's bounding box is degenerate (flat) in at least one direction so a box cannot be created.")
	  Return Result.Failure
	End If
	Dim brep = Brep.CreateFromBox(bbox)
	doc.Objects.AddBrep(brep)
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
