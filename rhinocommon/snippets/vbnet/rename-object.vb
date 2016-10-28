Partial Friend Class Examples
  Public Shared Function RenameObject(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select object to change name", True, ObjectType.AnyObject, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim rhino_object = obj_ref.Object()

	Dim new_object_name = ""
	rc = RhinoGet.GetString("New object name", True, new_object_name)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	If String.IsNullOrWhiteSpace(new_object_name) Then
	  Return Result.Nothing
	End If

	If rhino_object.Name <> new_object_name Then
	  rhino_object.Attributes.Name = new_object_name
	  rhino_object.CommitChanges()
	End If

	Return Result.Success
  End Function
End Class
