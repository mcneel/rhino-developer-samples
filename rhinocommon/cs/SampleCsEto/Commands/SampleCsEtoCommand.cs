using Rhino;
using Rhino.Commands;

namespace SampleCsEto.Commands
{
  public class SampleCsEtoCommand : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsEto"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoApp.WriteLine("{0} plug-in loaded.", SampleCsEtoPlugIn.Instance.Name);
      return Result.Success;
    }
  }
}
