using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using SampleCsWinForms.Forms;

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
      var result = Result.Cancel;

      if (mode == RunMode.Interactive)
      {
        var form = new SampleCsModalForm { StartPosition = FormStartPosition.CenterParent };
        var dialog_result = Rhino.UI.Dialogs.ShowSemiModal(form);
        if (dialog_result == DialogResult.OK)
          result = Result.Success;
      }
      else
      {
        var msg = string.Format("Scriptable version of {0} command not implemented.", EnglishName);
        RhinoApp.WriteLine(msg);
      }

      return result;
    }
  }
}
