Partial Friend Class Examples
  Public Shared Function MovePointObjects(ByVal doc As RhinoDoc) As Result
	Dim obj_refs() As ObjRef = Nothing
	Dim rc = RhinoGet.GetMultipleObjects("Select points to move", False, ObjectType.Point, obj_refs)
	If rc IsNot Result.Success OrElse obj_refs Is Nothing Then
	  Return rc
	End If

	Dim gp = New GetPoint()
	gp.SetCommandPrompt("Point to move from")
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim start_point = gp.Point()

	gp.SetCommandPrompt("Point to move to")
	gp.SetBasePoint(start_point, False)
	gp.DrawLineFromPoint(start_point, True)
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim end_point = gp.Point()

	Dim xform = Transform.Translation(end_point - start_point)

	For Each obj_ref In obj_refs
	  doc.Objects.Transform(obj_ref, xform, True)
	Next obj_ref

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
