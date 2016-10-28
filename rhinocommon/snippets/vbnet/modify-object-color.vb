Partial Friend Class Examples
  Public Shared Function ModifyObjectColor(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select object", False, ObjectType.AnyObject, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim rhino_object = obj_ref.Object()
	Dim color = rhino_object.Attributes.ObjectColor
	Dim b As Boolean = Rhino.UI.Dialogs.ShowColorDialog(color)
	If Not b Then
		Return Result.Cancel
	End If

	rhino_object.Attributes.ObjectColor = color
	rhino_object.Attributes.ColorSource = ObjectColorSource.ColorFromObject
	rhino_object.CommitChanges()

	' an object's color attributes can also be specified
	' when the object is added to Rhino
	Dim sphere = New Sphere(Point3d.Origin, 5.0)
	Dim attributes = New ObjectAttributes()
	attributes.ObjectColor = System.Drawing.Color.CadetBlue
	attributes.ColorSource = ObjectColorSource.ColorFromObject
	doc.Objects.AddSphere(sphere, attributes)

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
