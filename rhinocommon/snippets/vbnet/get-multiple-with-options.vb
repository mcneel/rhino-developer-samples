Partial Friend Class Examples
  Public Shared Function GetMultipleWithOptions(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Const geometryFilter As Rhino.DocObjects.ObjectType = Rhino.DocObjects.ObjectType.Surface Or Rhino.DocObjects.ObjectType.PolysrfFilter Or Rhino.DocObjects.ObjectType.Mesh
	Dim integer1 As Integer = 300
	Dim integer2 As Integer = 300

	Dim optionInteger1 As New Rhino.Input.Custom.OptionInteger(integer1, 200, 900)
	Dim optionInteger2 As New Rhino.Input.Custom.OptionInteger(integer2, 200, 900)

	Dim go As New Rhino.Input.Custom.GetObject()
	go.SetCommandPrompt("Select surfaces, polysurfaces, or meshes")
	go.GeometryFilter = geometryFilter
	go.AddOptionInteger("Option1", optionInteger1)
	go.AddOptionInteger("Option2", optionInteger2)
	go.GroupSelect = True
	go.SubObjectSelect = False
	go.EnableClearObjectsOnEntry(False)
	go.EnableUnselectObjectsOnExit(False)
	go.DeselectAllBeforePostSelect = False

	Dim bHavePreselectedObjects As Boolean = False

	Do
	  Dim res As Rhino.Input.GetResult = go.GetMultiple(1, 0)

	  If res Is Rhino.Input.GetResult.Option Then
		go.EnablePreSelect(False, True)
		Continue Do

	  ElseIf res IsNot Rhino.Input.GetResult.Object Then
		Return Rhino.Commands.Result.Cancel
	  End If

	  If go.ObjectsWerePreselected Then
		bHavePreselectedObjects = True
		go.EnablePreSelect(False, True)
		Continue Do
	  End If

	  Exit Do
	Loop

	If bHavePreselectedObjects Then
	  ' Normally when command finishes, pre-selected objects will remain
	  ' selected, when and post-selected objects will be unselected.
	  ' With this sample, it is possible to have a combination of 
	  ' pre-selected and post-selected objects. To make sure everything
	  ' "looks the same", unselect everything before finishing the command.
	  For i As Integer = 0 To go.ObjectCount - 1
		Dim rhinoObject As Rhino.DocObjects.RhinoObject = go.Object(i).Object()
		If Nothing IsNot rhinoObject Then
		  rhinoObject.Select(False)
		End If
	  Next i
	  doc.Views.Redraw()
	End If

	Dim objectCount As Integer = go.ObjectCount
	integer1 = optionInteger1.CurrentValue
	integer2 = optionInteger2.CurrentValue

	Rhino.RhinoApp.WriteLine("Select object count = {0}", objectCount)
	Rhino.RhinoApp.WriteLine("Value of integer1 = {0}", integer1)
	Rhino.RhinoApp.WriteLine("Value of integer2 = {0}", integer2)

	Return Rhino.Commands.Result.Success
  End Function
End Class
