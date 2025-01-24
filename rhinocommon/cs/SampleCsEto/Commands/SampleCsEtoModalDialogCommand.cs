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
      Result rc = Result.Cancel;

      if (mode == RunMode.Interactive)
      {
        Views.SampleCsEtoModalDialog dialog = new Views.SampleCsEtoModalDialog();
        Eto.Forms.DialogResult dialog_rc = dialog.ShowModal(RhinoEtoApp.MainWindow);
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
