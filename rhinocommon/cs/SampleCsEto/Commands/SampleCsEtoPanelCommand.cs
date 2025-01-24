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
      System.Guid panel_id = Views.SampleCsEtoPanel.PanelId;
      bool visible = Panels.IsPanelVisible(panel_id);

      string prompt = (visible)
        ? "Sample panel is visible. New value"
        : "Sample panel is hidden. New value";

      GetOption go = new GetOption();
      go.SetCommandPrompt(prompt);
      int hide_index = go.AddOption("Hide");
      int show_index = go.AddOption("Show");
      int toggle_index = go.AddOption("Toggle");
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      CommandLineOption option = go.Option();
      if (null == option)
        return Result.Failure;

      int index = option.Index;
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
