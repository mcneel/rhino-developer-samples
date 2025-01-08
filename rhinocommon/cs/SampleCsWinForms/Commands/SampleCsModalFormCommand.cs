using Rhino;
using Rhino.Commands;
using RhinoWindows;
using System.Windows.Forms;

namespace SampleCsWinForms.Commands
{
  [System.Runtime.InteropServices.Guid("79a8c6e4-b0c7-44fe-9faf-e9f041004f74")]
  public class SampleCsModalFormCommand : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsModalForm"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Result rc = Result.Cancel;

      if (mode == RunMode.Interactive)
      {
        Forms.SampleCsModalForm form = new Forms.SampleCsModalForm { StartPosition = FormStartPosition.CenterParent };
        DialogResult dialog_result = form.ShowDialog(RhinoWinApp.MainWindow);
        if (dialog_result == DialogResult.OK)
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
