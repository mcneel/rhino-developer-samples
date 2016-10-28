Partial Friend Class Examples
  Public Shared Function EditText(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select text", False, Rhino.DocObjects.ObjectType.Annotation, objref)
	If rc IsNot Rhino.Commands.Result.Success OrElse objref Is Nothing Then
	  Return rc
	End If

	Dim textobj As Rhino.DocObjects.TextObject = TryCast(objref.Object(), Rhino.DocObjects.TextObject)
	If textobj Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim textentity As Rhino.Geometry.TextEntity = TryCast(textobj.Geometry, Rhino.Geometry.TextEntity)
	If textentity Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If
	Dim str As String = textentity.Text
	rc = Rhino.Input.RhinoGet.GetString("New text", False, str)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	textentity.Text = str
	textobj.CommitChanges()
	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
