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
      Result rc = Result.Cancel;

      if (mode == RunMode.Interactive)
      {
        Views.SampleCsEtoSemiModalDialog dialog = new Views.SampleCsEtoSemiModalDialog();
        Eto.Forms.DialogResult dialog_rc = dialog.ShowSemiModal(doc, RhinoEtoApp.MainWindow);
        if (dialog_rc == Eto.Forms.DialogResult.Ok)
          rc = Result.Success;
      }
      else
      {
        string msg = string.Format("Scriptable version of {0} command not implemented.", EnglishName);
        RhinoApp.WriteLine(msg);
      }

      return rc;
    }
  }
}
