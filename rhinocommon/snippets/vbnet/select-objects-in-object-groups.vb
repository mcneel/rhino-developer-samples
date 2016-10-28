Partial Friend Class Examples
  Public Shared Function SelectObjectsInObjectGroups(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rs = RhinoGet.GetOneObject("Select object", False, ObjectType.AnyObject, obj_ref)
	If rs IsNot Result.Success Then
	  Return rs
	End If
	Dim rhino_object = obj_ref.Object()
	If rhino_object Is Nothing Then
	  Return Result.Failure
	End If

	Dim rhino_object_groups = rhino_object.Attributes.GetGroupList().DefaultIfEmpty(-1)

	Dim selectable_objects = From obj In doc.Objects.GetObjectList(ObjectType.AnyObject)
	                         Where obj.IsSelectable(True, False, False, False)
	                         Select obj

	For Each selectable_object In selectable_objects
	  For Each group In selectable_object.Attributes.GetGroupList()
		If rhino_object_groups.Contains(group) Then
			selectable_object.Select(True)
			Continue For
		End If
	  Next group
	Next selectable_object
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
