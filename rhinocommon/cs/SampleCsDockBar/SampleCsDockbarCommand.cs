using Rhino;
using Rhino.Commands;

namespace SampleCsDockBar
{
  public class SampleCsDockbarCommand : Command
  {
    public override string EnglishName => "SampleCsDockBar";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoWindows.Controls.DockBar.Show(WinFormsDockBar.BarId, false);
      RhinoWindows.Controls.DockBar.Show(WpfDockBar.BarId, false);
      return Result.Success;
    }
  }
}
