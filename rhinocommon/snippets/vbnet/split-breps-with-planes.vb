Partial Friend Class Examples
  Public Shared Function SplitBrepsWithPlane(ByVal doc As RhinoDoc) As Result
	'First, collect all the breps to split
	Dim obj_refs() As ObjRef = Nothing
	Dim rc = RhinoGet.GetMultipleObjects("Select breps to split", False, ObjectType.Brep, obj_refs)
	If rc IsNot Result.Success OrElse obj_refs Is Nothing Then
	  Return rc
	End If

	' Get the final plane
	Dim plane As Plane = Nothing
	rc = RhinoGet.GetPlane(plane)
	If rc IsNot Result.Success Then
	  Return rc
	End If

	'Iterate over all object references
	For Each obj_ref In obj_refs
	  Dim brep = obj_ref.Brep()
	  Dim bbox = brep.GetBoundingBox(False)

	  'Grow the boundingbox in all directions
	  'If the boundingbox is flat (zero volume or even zero area)
	  'then the CreateThroughBox method will fail.
	  Dim min_point = bbox.Min
	  min_point.X -= 1.0
	  min_point.Y -= 1.0
	  min_point.Z -= 1.0
	  bbox.Min = min_point
	  Dim max_point = bbox.Max
	  max_point.X += 1.0
	  max_point.Y += 1.0
	  max_point.Z += 1.0
	  bbox.Max = max_point

	  Dim plane_surface = PlaneSurface.CreateThroughBox(plane, bbox)
	  If plane_surface Is Nothing Then
		'This is rare, it will most likely not happen unless either the plane or the boundingbox are invalid
		RhinoApp.WriteLine("Cutting plane could not be constructed.")
	  Else
		Dim breps = brep.Split(plane_surface.ToBrep(), doc.ModelAbsoluteTolerance)
		If breps Is Nothing OrElse breps.Length = 0 Then
		  RhinoApp.Write("Plane does not intersect brep (id:{0})", obj_ref.ObjectId)
		  Continue For
		End If
		For Each brep_piece In breps
		  doc.Objects.AddBrep(brep_piece)
		Next brep_piece
		doc.Objects.AddSurface(plane_surface)
		doc.Objects.Delete(obj_ref, False)
	  End If
	Next obj_ref

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
