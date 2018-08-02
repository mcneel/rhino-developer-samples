using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.UI;
using SampleCsWinForms.Forms;

namespace SampleCsWinForms.Commands
{
  [System.Runtime.InteropServices.Guid("3d3db472-509d-41c5-97e3-2890c5fb9827")]
  public class SampleCsPanelCommand : Command
  {
    public override string EnglishName => "SampleCsPanel";

    protected override Result RunCommand(Rhino.RhinoDoc doc, RunMode mode)
    {
      var panel_id = SampleCsPanelUserControl.PanelId;
      var visible = Panels.IsPanelVisible(panel_id);

      var prompt = visible
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
        if (visible)
          Panels.ClosePanel(panel_id);
      }
      else if (index == show_index)
      {
        if (!visible)
          Panels.OpenPanel(panel_id);
      }
      else if (index == toggle_index)
      {
        if (visible)
          Panels.ClosePanel(panel_id);
        else
          Panels.OpenPanel(panel_id);
      }

      return Result.Success;
    }
  }
}
