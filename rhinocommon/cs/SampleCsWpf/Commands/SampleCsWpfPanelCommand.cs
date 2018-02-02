using System.Runtime.InteropServices;
using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.UI;
using SampleCsWpf.Views;

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
      Panels.RegisterPanel(SampleCsWpfPlugIn.Instance, typeof(SampleCsWpfPanelHost), "SampleWpfPanel", System.Drawing.SystemIcons.WinLogo);
    }

    public static SampleCsWpfPanelCommand Instance
    {
      get; private set;
    }

    public override string EnglishName => "SampleCsWpfPanel";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var panel_id = typeof(SampleCsWpfPanelHost).GUID;
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
