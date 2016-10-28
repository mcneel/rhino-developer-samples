Partial Friend Class Examples
  Public Shared Function MovePointObjectsNonUniform(ByVal doc As RhinoDoc) As Result
	Dim obj_refs() As ObjRef = Nothing
	Dim rc = RhinoGet.GetMultipleObjects("Select points to move", False, ObjectType.Point, obj_refs)
	If rc IsNot Result.Success OrElse obj_refs Is Nothing Then
	  Return rc
	End If

	For Each obj_ref In obj_refs
	  Dim point3d = obj_ref.Point().Location
	  ' modify the point coordinates in some way ...
	  point3d.X += 1
	  point3d.Y += 1
	  point3d.Z += 1

	  doc.Objects.Replace(obj_ref, point3d)
	Next obj_ref

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
