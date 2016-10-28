Partial Friend Class Examples
  Public Shared Function GetUUID(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select object", False, ObjectType.AnyObject, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	If obj_ref Is Nothing Then
	  Return Result.Nothing
	End If

	Dim uuid = obj_ref.ObjectId
	RhinoApp.WriteLine("The object's unique id is {0}", uuid.ToString())
	Return Result.Success
  End Function
End Class
