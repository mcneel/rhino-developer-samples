Partial Friend Class Examples
  Public Shared Function DuplicateObject(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select object to duplicate", False, ObjectType.AnyObject, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim rhino_object = obj_ref.Object()

	Dim geometry_base = rhino_object.DuplicateGeometry()
	If geometry_base IsNot Nothing Then
	  If doc.Objects.Add(geometry_base) <> Guid.Empty Then
		doc.Views.Redraw()
	  End If
	End If

	Return Result.Success
  End Function
End Class
