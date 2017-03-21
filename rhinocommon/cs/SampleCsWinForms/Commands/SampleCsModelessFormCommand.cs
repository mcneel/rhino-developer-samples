using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using SampleCsWinForms.Forms;

namespace SampleCsWinForms.Commands
{
  [System.Runtime.InteropServices.Guid("a8b8b5d1-1b26-4ef0-941f-801e0f9b3323")]
  public class SampleCsModelessFormCommand : Command
  {
    private SampleCsModelessForm Form { get; set; }

    public override string EnglishName
    {
      get { return "SampleCsModelessForm"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      if (null == Form)
      {
        Form = new SampleCsModelessForm();
        Form.FormClosed += OnFormClosed;
        Form.Show(RhinoApp.MainWindow());
      }

      return Result.Success;
    }

    private void OnFormClosed(object sender, FormClosedEventArgs e)
    {
      Form.Dispose();
      Form = null;
    }
  }
}
