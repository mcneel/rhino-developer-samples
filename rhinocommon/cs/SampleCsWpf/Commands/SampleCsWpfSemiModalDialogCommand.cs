using Rhino;
using Rhino.Commands;
using RhinoWindows;

namespace SampleCsWpf.Commands
{
  public class SampleCsWpfSemiModalDialogCommand : Command
  {
    public override string EnglishName => "SampleCsWpfSemiModalDialog";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var dialog = new Views.SampleCsWpfDialog();
      dialog.ShowSemiModal(RhinoApp.MainWindowHandle());

      return Result.Success;
    }
  }
}