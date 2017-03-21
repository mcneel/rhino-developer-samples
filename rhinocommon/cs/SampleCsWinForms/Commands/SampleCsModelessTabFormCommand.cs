using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using SampleCsWinForms.Forms;

namespace SampleCsWinForms.Commands
{
  [System.Runtime.InteropServices.Guid("32d03e4e-c9e1-46dd-ba0c-8a570a7255e1")]
  public class SampleCsModelessTabFormCommand : Command
  {
    private Forms.SampleCsModelessTabForm Form { get; set; }

    public override string EnglishName
    {
      get { return "SampleCsModelessTabForm"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      if (null == Form)
      {
        Form = new SampleCsModelessTabForm();
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
