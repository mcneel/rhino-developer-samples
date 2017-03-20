using System;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("771a4066-071b-480c-bd34-7fd7a2cd052b")]
  public class SampleCsSelectLayerObjects : Command
  {
    public SampleCsSelectLayerObjects()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsSelectLayerObjects"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      int layerIndex = -1;
      bool currentState = false;
      DialogResult rc = Rhino.UI.Dialogs.ShowSelectLayerDialog(ref layerIndex, "Select Layer Objects", false, false, ref currentState);
      if (rc != DialogResult.OK)
        return Result.Cancel;

      if (layerIndex >= 0 && layerIndex < doc.Layers.Count)
      {
        Rhino.DocObjects.Layer layer = doc.Layers[layerIndex];
        if (layer.IsDeleted)
          return Result.Nothing;

        if (!layer.IsVisible)
        {
          RhinoApp.WriteLine("{0} layer is hidden.", layer.Name);
          return Result.Nothing;
        }

        Rhino.DocObjects.RhinoObject[] rh_objects = doc.Objects.FindByLayer(layer);
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
