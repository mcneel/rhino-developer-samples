using System.Linq;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsLayerOff : Command
  {
    public override string EnglishName => "SampleCsLayerOff";

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

  public class SampleCsLayerOffInDetails : Command
  {
    public override string EnglishName => "SampleCsLayerOffInDetails";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Find the layer index by full path name
      var layer_index = doc.Layers.FindByFullPath("TO-DRAW", -1);
      if (-1 == layer_index)
        return Result.Cancel;

      // Get the layer object
      var layer = doc.Layers[layer_index];
      if (null == layer)
        return Result.Failure;

      // If the layer is off globally, then there is nothing to do
      if (!layer.IsVisible)
        return Result.Cancel;

      // Find the page view
      var page_view = doc.Views.GetPageViews().First(
        item => item.PageName.Equals("PANEL_01", System.StringComparison.OrdinalIgnoreCase)
        );

      if (null == page_view)
        return Result.Cancel;

      // Process each detail
      foreach (var detail in page_view.GetDetailViews())
      {
        var viewport_id = detail.Viewport.Id;

        // Re-acquire the layer object, as the underlying
        // object may have been modified
        layer = doc.Layers[layer_index];
        if (null != layer)
        {
          // If the layer is visible on the detail, turn it off
          if (layer.PerViewportIsVisible(viewport_id))
          {
            layer.SetPerViewportVisible(viewport_id, false);
            layer.SetPerViewportPersistentVisibility(viewport_id, false);
          }
        }
      }

      // Redraw if needed
      var view = doc.Views.ActiveView;
      if (null != view && view.ActiveViewportID == page_view.ActiveViewportID)
        doc.Views.Redraw();

      return Result.Success;
    }
  }
}
