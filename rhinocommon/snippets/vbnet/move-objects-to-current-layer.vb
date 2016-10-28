Partial Friend Class Examples
  Public Shared Function MoveObjectsToCurrentLayer(ByVal doc As RhinoDoc) As Result
	' all non-light objects that are selected
	Dim object_enumerator_settings = New ObjectEnumeratorSettings()
	object_enumerator_settings.IncludeLights = False
	object_enumerator_settings.IncludeGrips = True
	object_enumerator_settings.NormalObjects = True
	object_enumerator_settings.LockedObjects = True
	object_enumerator_settings.HiddenObjects = True
	object_enumerator_settings.ReferenceObjects = True
	object_enumerator_settings.SelectedObjectsFilter = True
	Dim selected_objects = doc.Objects.GetObjectList(object_enumerator_settings)

	Dim current_layer_index = doc.Layers.CurrentLayerIndex
	For Each selected_object In selected_objects
	  selected_object.Attributes.LayerIndex = current_layer_index
	  selected_object.CommitChanges()
	Next selected_object
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
