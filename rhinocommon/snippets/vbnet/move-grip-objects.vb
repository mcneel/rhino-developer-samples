Partial Friend Class Examples
  Public Shared Function MoveGripObjects(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	' The following example demonstrates how to move a surface's grip objects.
	' In this sample, all grips will be moved a fixed distance of 0.5 units 
	' in the normal direction of the surface at that grip location.

	Dim objRef As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select surface for control point editing", False, Rhino.DocObjects.ObjectType.Surface, objRef)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim obj As Rhino.DocObjects.RhinoObject = objRef.Object()
	If Nothing Is obj Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim srf As Rhino.Geometry.Surface = objRef.Surface()
	If Nothing Is srf Then
	  Return Rhino.Commands.Result.Failure
	End If

	' Make sure the object's grips are enabled
	obj.GripsOn = True
	doc.Views.Redraw()

	Dim grips() As Rhino.DocObjects.GripObject = obj.GetGrips()
	For i As Integer = 0 To grips.Length - 1
	  Dim grip As Rhino.DocObjects.GripObject = grips(i)

	  ' Calculate the point on the surface closest to our test point,
	  ' which is the grip's 3-D location (for this example).
	  Dim u As Double = Nothing, v As Double = Nothing
	  If srf.ClosestPoint(grip.CurrentLocation, u, v) Then
		' Compute the surface normal at a point
		Dim dir As Rhino.Geometry.Vector3d = srf.NormalAt(u, v)
		dir.Unitize()

		' Scale by our fixed distance
		dir *= 0.5

		' Move the grip to a new location
		grip.Move(dir)
	  End If
	Next i

	' Altered grip positions on a RhinoObject are used to calculate an updated
	' object that is added to the document.
	doc.Objects.GripUpdate(obj, False)
	doc.Views.Redraw()

	Return Rhino.Commands.Result.Success
  End Function

End Class
