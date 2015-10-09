Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class MoveSelectedObjectsToCurrentLayerCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbMoveSelectedObjectsToCurrentLayer"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
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
      For Each selected_object As RhinoObject In selected_objects
        selected_object.Attributes.LayerIndex = current_layer_index
        selected_object.CommitChanges()
      Next
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace