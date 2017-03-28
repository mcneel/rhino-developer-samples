using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.UI;

namespace SampleCsEto.Commands
{
  public class SampleCsEtoPanelCommand : Command
  {
    public SampleCsEtoPanelCommand()
    {
      Panels.RegisterPanel(PlugIn, typeof(Views.SampleCsEtoPanel), "Sample", Properties.Resources.SampleCsEtoPanel);
      Instance = this;
    }

    public static SampleCsEtoPanelCommand Instance
    {
      get;
      private set;
    }

    public override string EnglishName
    {
      get { return "SampleCsEtoPanel"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var panel_id = Views.SampleCsEtoPanel.PanelId;
      var visible = Panels.IsPanelVisible(panel_id);

      var prompt = (visible)
        ? "Sample panel is visible. New value"
        : "Sample panel is hidden. New value";

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
