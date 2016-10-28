Partial Class Examples
  Public Shared Function AddChildLayer(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' Get an existing layer
    Dim default_name As String = doc.Layers.CurrentLayer.Name

    ' Prompt the user to enter a layer name
    Dim gs As New Rhino.Input.Custom.GetString()
    gs.SetCommandPrompt("Name of existing layer")
    gs.SetDefaultString(default_name)
    gs.AcceptNothing(True)
    gs.[Get]()
    If gs.CommandResult() <> Rhino.Commands.Result.Success Then
      Return gs.CommandResult()
    End If

    ' Was a layer named entered?
    Dim layer_name As String = gs.StringResult().Trim()
    Dim index As Integer = doc.Layers.Find(layer_name, True)
    If index < 0 Then
      Return Rhino.Commands.Result.Cancel
    End If

    Dim parent_layer As Rhino.DocObjects.Layer = doc.Layers(index)

    ' Create a child layer
    Dim child_name As String = parent_layer.Name + "_child"
    Dim childlayer As New Rhino.DocObjects.Layer()
    childlayer.ParentLayerId = parent_layer.Id
    childlayer.Name = child_name
    childlayer.Color = System.Drawing.Color.Red

    index = doc.Layers.Add(childlayer)
    If index < 0 Then
      Rhino.RhinoApp.WriteLine("Unable to add {0} layer.", child_name)
      Return Rhino.Commands.Result.Failure
    End If
    Return Rhino.Commands.Result.Success
  End Function
End Class