using Rhino;
using Rhino.Commands;
using RhinoWindows;
using RhinoWindows.Forms;
using System.Windows.Forms;

namespace SampleCsWinForms.Commands
{
  [System.Runtime.InteropServices.Guid("85cb95cc-1387-4729-8e03-62d9279c50a0")]
  public class SampleCsSemiModalFormCommand : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsSemiModalForm"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Result result = Result.Cancel;

      if (mode == RunMode.Interactive)
      {
        Forms.SampleCsModalForm form = new Forms.SampleCsModalForm { StartPosition = FormStartPosition.CenterParent };
        DialogResult dialog_result = form.ShowSemiModal(RhinoWinApp.MainWindow);
        if (dialog_result == DialogResult.OK)
          result = Result.Success;
      }
      else
      {
        string msg = string.Format("Scriptable version of {0} command not implemented.", EnglishName);
        RhinoApp.WriteLine(msg);
      }

      return result;
    }
  }
}
