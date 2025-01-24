using Rhino;
using Rhino.Commands;
using SampleCsWinForms.Forms;
using System.Windows.Forms;

namespace SampleCsWinForms.Commands
{
  [System.Runtime.InteropServices.Guid("3ced80e7-c834-45bf-802e-e71512d08583")]
  public class SampleCsFibonacciCommand : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsFibonacci"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      SampleCsFibonacciForm form = new SampleCsFibonacciForm { StartPosition = FormStartPosition.CenterParent };
      form.ShowDialog();
      return Result.Success;
    }
  }
}
