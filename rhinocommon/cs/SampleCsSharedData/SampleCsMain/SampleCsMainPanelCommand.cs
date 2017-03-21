using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.UI;

namespace SampleCsMain
{
  [System.Runtime.InteropServices.Guid("9a91bb39-1979-4d48-90ae-7dc9303579a2")]
  public class SampleCsMainPanelCommand : Command
  {
    /// <summary>
    /// Public constructor.
    /// </summary>
    public SampleCsMainPanelCommand()
    {
    }

    /// <summary>
    /// Returns the English command name.
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsMainPanel"; }
    }

    /// <summary>
    /// Called by Rhino when the user wants to run the command.
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      System.Guid panel_id = SampleCsMainPanel.PanelId;
      bool visible = Panels.IsPanelVisible(panel_id);

      string prompt = (visible)
        ? "Main panel is visible. New value"
        : "Main Manager panel is hidden. New value";

      GetOption go = new GetOption();
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
