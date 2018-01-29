using Rhino;
using Rhino.Commands;

namespace SampleCsWithLicense
{
  public class SampleCsWithLicenseCommand : Command
  {
    public override string EnglishName => "SampleCsWithLicense";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoApp.WriteLine($"{0} plug-in loaded.", SampleCsWithLicensePlugIn.Instance.Name);
      return Result.Success;
    }
  }
}
