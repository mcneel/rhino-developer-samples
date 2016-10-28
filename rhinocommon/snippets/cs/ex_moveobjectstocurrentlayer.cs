using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace examples_cs
{
  public class MoveSelectedObjectsToCurrentLayerCommand : Command
  {
    public override string EnglishName
    {
      get { return "csMoveSelectedObjectsToCurrentLayer"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // all non-light objects that are selected
      var object_enumerator_settings = new ObjectEnumeratorSettings();
      object_enumerator_settings.IncludeLights = false;
      object_enumerator_settings.IncludeGrips = true;
      object_enumerator_settings.NormalObjects = true;
      object_enumerator_settings.LockedObjects = true;
      object_enumerator_settings.HiddenObjects = true;
      object_enumerator_settings.ReferenceObjects = true;
      object_enumerator_settings.SelectedObjectsFilter = true;
      var selected_objects = doc.Objects.GetObjectList(object_enumerator_settings);

      var current_layer_index = doc.Layers.CurrentLayerIndex;
      foreach (var selected_object in selected_objects)
      {
        selected_object.Attributes.LayerIndex = current_layer_index;
        selected_object.CommitChanges();
      }
      doc.Views.Redraw();
      return Result.Success;
    }
  }
}