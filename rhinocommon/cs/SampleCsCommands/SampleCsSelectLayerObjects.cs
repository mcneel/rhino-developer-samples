using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace SampleCsCommands
{
  public class SampleCsSelectLayerObjects : Command
  {
    public override string EnglishName => "SampleCsSelectLayerObjects";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      int layer_index = -1;
      bool current_state = false;
      bool rc = Rhino.UI.Dialogs.ShowSelectLayerDialog(ref layer_index, "Select Layer Objects", false, false, ref current_state);
      if (!rc)
        return Result.Cancel;

      if (layer_index >= 0 && layer_index < doc.Layers.Count)
      {
        Layer layer = doc.Layers[layer_index];
        if (layer.IsDeleted)
          return Result.Nothing;

        if (!layer.IsVisible)
        {
          RhinoApp.WriteLine("{0} layer is hidden.", layer.Name);
          return Result.Nothing;
        }

        RhinoObject[] rh_objects = doc.Objects.FindByLayer(layer);
        foreach (RhinoObject rh_object in rh_objects)
        {
          if (rh_object.IsSelectable())
            rh_object.Select(true);
        }

        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
