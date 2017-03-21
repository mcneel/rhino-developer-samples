using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.UI;

namespace SampleCsWpf.Commands
{
  [System.Runtime.InteropServices.Guid("664de49d-11f7-4de8-9c7f-26f768685e21")]
  public class SampleCsWpfPanel : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsWpfPanel"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var panel_id = SampleCsWpfPanelHost.PanelId;
      var panel_visible = Panels.IsPanelVisible(panel_id);

      var prompt = (panel_visible)
        ? "Sample panel is visible. New value"
        : "Sample Manager panel is hidden. New value";

      var go = new GetOption();
      go.SetCommandPrompt(prompt);
      var hide_index = go.AddOption("Hide");
      var show_index = go.AddOption("Show");
      var toggle_index = go.AddOption("Toggle");
      go.Get();

      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var option = go.Option();
      if (null == option)
        return Result.Failure;

      var index = option.Index;
      if (index == hide_index)
      {
        if (panel_visible)
          Panels.ClosePanel(panel_id);
      }
      else if (index == show_index)
      {
        if (!panel_visible)
          Panels.OpenPanel(panel_id);
      }
      else if (index == toggle_index)
      {
        if (panel_visible)
          Panels.ClosePanel(panel_id);
        else
          Panels.OpenPanel(panel_id);
      }
      return Result.Success;
    }
  }
}
