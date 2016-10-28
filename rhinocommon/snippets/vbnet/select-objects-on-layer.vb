Partial Friend Class Examples
  Public Shared Function SelLayer(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	' Prompt for a layer name
	Dim layername As String = doc.Layers.CurrentLayer.Name
	Dim rc As Result = Rhino.Input.RhinoGet.GetString("Name of layer to select objects", True, layername)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	' Get all of the objects on the layer. If layername is bogus, you will
	' just get an empty list back
	Dim rhobjs() As Rhino.DocObjects.RhinoObject = doc.Objects.FindByLayer(layername)
	If rhobjs Is Nothing OrElse rhobjs.Length < 1 Then
	  Return Rhino.Commands.Result.Cancel
	End If

	For i As Integer = 0 To rhobjs.Length - 1
	  rhobjs(i).Select(True)
	Next i
	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
