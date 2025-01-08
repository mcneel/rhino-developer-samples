using Rhino;
using Rhino.Commands;

namespace SampleCsSettings2
{
  public class SampleCsSettings2Command : Command
  {
    public override string EnglishName => "SampleCsSettings2";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      SampleCsSettings2PlugIn.Instance.ReadSetting();
      return Result.Success;
    }
  }
}
