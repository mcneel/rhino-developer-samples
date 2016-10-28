Partial Friend Class Examples
  Public Shared Function RenameLayer(ByVal doc As RhinoDoc) As Result
	Dim layer_name As String = ""
	Dim rc = RhinoGet.GetString("Name of layer to rename", True, layer_name)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	If String.IsNullOrWhiteSpace(layer_name) Then
	  Return Result.Nothing
	End If

	' because of sublayers it's possible that more than one layer has the same name
	' so simply calling doc.Layers.Find(layerName) isn't good enough.  If "layerName" returns
	' more than one layer then present them to the user and let him decide.
	Dim matching_layers = (
	    From layer In doc.Layers
	    Where layer.Name = layer_name
	    Select layer).ToList()

	Dim layer_to_rename As Layer = Nothing
	If matching_layers.Count = 0 Then
	  RhinoApp.WriteLine("Layer ""{0}"" does not exist.", layer_name)
	  Return Result.Nothing
	ElseIf matching_layers.Count = 1 Then
	  layer_to_rename = matching_layers(0)
	ElseIf matching_layers.Count > 1 Then
	  For i As Integer = 0 To matching_layers.Count - 1
		RhinoApp.WriteLine("({0}) {1}", i+1, matching_layers(i).FullPath.Replace("::", "->"))
	  Next i
	  Dim selected_layer As Integer = -1
	  rc = RhinoGet.GetInteger("which layer?", True, selected_layer)
	  If rc IsNot Result.Success Then
		Return rc
	  End If
	  If selected_layer > 0 AndAlso selected_layer <= matching_layers.Count Then
		layer_to_rename = matching_layers(selected_layer - 1)
	  Else
		  Return Result.Nothing
	  End If
	End If

	If layer_to_rename Is Nothing Then
	  Return Result.Nothing
	End If

	layer_name = ""
	rc = RhinoGet.GetString("New layer name", True, layer_name)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	If String.IsNullOrWhiteSpace(layer_name) Then
	  Return Result.Nothing
	End If

	layer_to_rename.Name = layer_name
	If Not layer_to_rename.CommitChanges() Then
	  Return Result.Failure
	End If
	Return Result.Success
  End Function
End Class
