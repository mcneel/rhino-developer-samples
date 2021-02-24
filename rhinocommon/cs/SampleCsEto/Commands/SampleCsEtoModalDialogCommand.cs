using Rhino;
using Rhino.Commands;
using Rhino.UI;

namespace SampleCsEto.Commands
{
  public class SampleCsEtoModalDialogCommand : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsEtoModalDialog"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var rc = Result.Cancel;

      if (mode == RunMode.Interactive)
      {
        var dialog = new Views.SampleCsEtoModalDialog();
        var dialog_rc = dialog.ShowModal(RhinoEtoApp.MainWindow);
        if (dialog_rc == Eto.Forms.DialogResult.Ok)
          rc = Result.Success;
      }
      else
      {
        var msg = string.Format("Scriptable version of {0} command not implemented.", EnglishName);
        RhinoApp.WriteLine(msg);
      }

      return rc;
    }
  }
}
