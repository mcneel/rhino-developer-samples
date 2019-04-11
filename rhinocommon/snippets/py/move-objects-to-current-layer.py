from Rhino import *
from Rhino.Commands import *
from Rhino.DocObjects import *
from scriptcontext import doc

def RunCommand():
    # all non-light objects that are selected
    object_enumerator_settings = ObjectEnumeratorSettings()
    object_enumerator_settings.IncludeLights = False
    object_enumerator_settings.IncludeGrips = True
    object_enumerator_settings.NormalObjects = True
    object_enumerator_settings.LockedObjects = True
    object_enumerator_settings.HiddenObjects = True
    object_enumerator_settings.ReferenceObjects = True
    object_enumerator_settings.SelectedObjectsFilter = True
    selected_objects = doc.Objects.GetObjectList(object_enumerator_settings)

    current_layer_index = doc.Layers.CurrentLayerIndex
    for selected_object in selected_objects:
        selected_object.Attributes.LayerIndex = current_layer_index
        selected_object.CommitChanges()

    doc.Views.Redraw()
    return Result.Success

if __name__ == "__main__":
    RunCommand()
