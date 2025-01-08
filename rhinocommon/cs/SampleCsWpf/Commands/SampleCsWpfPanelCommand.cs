using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.UI;
using SampleCsWpf.Views;
using System.Runtime.InteropServices;

namespace SampleCsWpf.Commands
{
  [Guid("0874C691-A083-4C05-9E31-96C2C63F7C7E")]
  public class SampleCsWpfPanelHost : RhinoWindows.Controls.WpfElementHost
  {
    public SampleCsWpfPanelHost(uint docSn)
      : base(new SampleCsWpfPanel(docSn), null)
    {
    }
  }

  public class SampleCsWpfPanelCommand : Command
  {
    public SampleCsWpfPanelCommand()
    {
      Instance = this;
      Panels.RegisterPanel(
        SampleCsWpfPlugIn.Instance,
        typeof(SampleCsWpfPanelHost),
        "SampleWpfPanel",
        System.Drawing.SystemIcons.WinLogo,
        PanelType.System
        );
    }

    public static SampleCsWpfPanelCommand Instance
    {
      get; private set;
    }

    public override string EnglishName => "SampleCsWpfPanel";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      System.Guid panel_id = typeof(SampleCsWpfPanelHost).GUID;

      if (mode == RunMode.Interactive)
      {
        Panels.OpenPanel(panel_id);
        return Result.Success;
      }

      bool panel_visible = Panels.IsPanelVisible(panel_id);

      string prompt = (panel_visible)
        ? "Sample panel is visible. New value"
        : "Sample Manager panel is hidden. New value";

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
