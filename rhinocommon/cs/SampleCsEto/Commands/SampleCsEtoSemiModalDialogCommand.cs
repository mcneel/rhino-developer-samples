using Rhino;
using Rhino.Commands;
using Rhino.UI;

namespace SampleCsEto.Commands
{
  public class SampleCsEtoSemiModalDialogCommand : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsEtoSemiModalDialog"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var rc = Result.Cancel;

      if (mode == RunMode.Interactive)
      {
        var dialog = new Views.SampleCsEtoSemiModalDialog();
        var dialog_rc = dialog.ShowSemiModal(doc, RhinoEtoApp.MainWindow);
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
