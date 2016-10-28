Partial Friend Class Examples
  Public Shared Function ModifyLightColor(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select light to change color", True, ObjectType.Light, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim light = obj_ref.Light()
	If light Is Nothing Then
	  Return Result.Failure
	End If

	Dim diffuse_color = light.Diffuse
	If Dialogs.ShowColorDialog(diffuse_color) Then
	  light.Diffuse = diffuse_color
	End If

	doc.Lights.Modify(obj_ref.ObjectId, light)
	Return Result.Success
  End Function
End Class
