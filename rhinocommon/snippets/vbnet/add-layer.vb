Partial Friend Class Examples
  Public Shared Function AddLayer(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	' Cook up an unused layer name
	Dim unused_name As String = doc.Layers.GetUnusedLayerName(False)

	' Prompt the user to enter a layer name
	Dim gs As New Rhino.Input.Custom.GetString()
	gs.SetCommandPrompt("Name of layer to add")
	gs.SetDefaultString(unused_name)
	gs.AcceptNothing(True)
	gs.Get()
	If gs.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return gs.CommandResult()
	End If

	' Was a layer named entered?
	Dim layer_name As String = gs.StringResult().Trim()
	If String.IsNullOrEmpty(layer_name) Then
	  Rhino.RhinoApp.WriteLine("Layer name cannot be blank.")
	  Return Rhino.Commands.Result.Cancel
	End If

	' Is the layer name valid?
	If Not Rhino.DocObjects.Layer.IsValidName(layer_name) Then
	  Rhino.RhinoApp.WriteLine(layer_name & " is not a valid layer name.")
	  Return Rhino.Commands.Result.Cancel
	End If

	' Does a layer with the same name already exist?
	Dim layer_index As Integer = doc.Layers.Find(layer_name, True)
	If layer_index >= 0 Then
	  Rhino.RhinoApp.WriteLine("A layer with the name {0} already exists.", layer_name)
	  Return Rhino.Commands.Result.Cancel
	End If

	' Add a new layer to the document
	layer_index = doc.Layers.Add(layer_name, System.Drawing.Color.Black)
	If layer_index < 0 Then
	  Rhino.RhinoApp.WriteLine("Unable to add {0} layer.", layer_name)
	  Return Rhino.Commands.Result.Failure
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
