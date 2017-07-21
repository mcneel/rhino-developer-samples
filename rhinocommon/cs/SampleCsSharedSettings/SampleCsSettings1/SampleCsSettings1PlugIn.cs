using Rhino.PlugIns;

namespace SampleCsSettings1
{
  public class SampleCsSettings1PlugIn : PlugIn
  {
    public SampleCsSettings1PlugIn()
    {
      Instance = this;
      // NOTE: Don't access plug-in settings in the constructor.
      // This will create a faulty settings manager behind the scenes
      // due to the plug-in ID not being correct at this time.
      // It is set only after the constructor has completed properly.
    }

    /// <summary>
    /// Gets the only instance of the SampleCsSettings1PlugIn plug-in.
    /// </summary>
    public static SampleCsSettings1PlugIn Instance
    {
      get; private set;
    }

    public override PlugInLoadTime LoadTime => PlugInLoadTime.WhenNeeded;

    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      Settings.SetInteger("SomeSetting", 0);
      return LoadReturnCode.Success;
    }
  }
}