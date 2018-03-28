using System.Windows.Forms;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsLayerOff : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsLayerOff"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var layer_index = doc.Layers.CurrentLayerIndex;
      var current_state = false;
      var res = Rhino.UI.Dialogs.ShowSelectLayerDialog(ref layer_index, "Select Layer", false, false, ref current_state);
      if (!res)
        return Result.Cancel;

      if (layer_index < 0 || layer_index >= doc.Layers.Count)
        return Result.Failure;

      if (layer_index == doc.Layers.CurrentLayerIndex)
        return Result.Nothing; // Cannot hide the current layer

      var layer = doc.Layers[layer_index];
      layer.IsVisible = false;
      layer.SetPersistentVisibility(false);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
