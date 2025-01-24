using Rhino;
using Rhino.Commands;

namespace SampleCsSettings1
{
  public class SampleCsSettings1Command : Command
  {
    public override string EnglishName => "SampleCsSettings1";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      SampleCsSettings1PlugIn plugin = SampleCsSettings1PlugIn.Instance;
      int value = plugin.Settings.GetInteger("SomeSetting");
      plugin.Settings.SetInteger("SomeSetting", ++value);
      RhinoApp.WriteLine($"SomeSetting is now {PlugIn.Settings.GetInteger("SomeSetting", -2)}");
      plugin.SaveSettings();
      return Result.Success;
    }
  }
}
