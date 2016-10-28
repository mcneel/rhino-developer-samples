Partial Friend Class Examples
  Public Shared Function ObjectColor(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objRefs() As Rhino.DocObjects.ObjRef = Nothing
	Dim cmdResult As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetMultipleObjects("Select objects to change color", False, Rhino.DocObjects.ObjectType.AnyObject, objRefs)
	If cmdResult IsNot Rhino.Commands.Result.Success Then
	  Return cmdResult
	End If

	Dim color As System.Drawing.Color = System.Drawing.Color.Black
	Dim rc As Boolean = Rhino.UI.Dialogs.ShowColorDialog(color)
	If Not rc Then
	  Return Rhino.Commands.Result.Cancel
	End If

	For i As Integer = 0 To objRefs.Length - 1
	  Dim obj As Rhino.DocObjects.RhinoObject = objRefs(i).Object()
	  If Nothing Is obj OrElse obj.IsReference Then
		Continue For
	  End If

	  If color <> obj.Attributes.ObjectColor Then
		obj.Attributes.ObjectColor = color
		obj.Attributes.ColorSource = Rhino.DocObjects.ObjectColorSource.ColorFromObject
		obj.CommitChanges()
	  End If
	Next i

	doc.Views.Redraw()

	Return Rhino.Commands.Result.Success
  End Function
End Class
