using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.UI;

namespace SampleCsEto.Commands
{
  /// <summary>
  /// Shows and hides the SampleCsEtoTabbedPanel panel
  /// </summary>
  public class SampleCsEtoTabbedPanelCommand : Command
  {
    /// <summary>
    /// Public constructor. The panel is registered here, when the plug-in
    /// loads and constructs its commands.
    /// </summary>
    public SampleCsEtoTabbedPanelCommand()
    {
      Panels.RegisterPanel(
        PlugIn,
        typeof(Views.SampleCsEtoTabbedPanel),
        "Sample Tabs",
        Properties.Resources.SampleCsEtoPanel,
        PanelType.PerDoc
        );
      Instance = this;
    }

    /// <summary>
    /// The only instance of this command
    /// </summary>
    public static SampleCsEtoTabbedPanelCommand Instance
    {
      get;
      private set;
    }

    /// <summary>
    /// The command name as it appears on the Rhino command line
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsEtoTabbedPanel"; }
    }

    /// <summary>
    /// Called by Rhino when the user runs the command
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      System.Guid panel_id = Views.SampleCsEtoTabbedPanel.PanelId;
      bool visible = Panels.IsPanelVisible(panel_id);

      string prompt = (visible)
        ? "Sample tabbed panel is visible. New value"
        : "Sample tabbed panel is hidden. New value";

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
