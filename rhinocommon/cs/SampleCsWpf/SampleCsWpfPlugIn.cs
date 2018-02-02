using Rhino.PlugIns;

namespace SampleCsWpf
{
  ///<summary>
  /// Every RhinoCommon plug-in must have one and only one Rhino.PlugIns.PlugIn
  /// inherited class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.
  ///</summary>
  public class SampleCsWpfPlugIn : Rhino.PlugIns.PlugIn
  {
    public SampleCsWpfPlugIn()
    {
      Instance = this;
    }

    /// <summary>
    /// Gets the only instance of the SampleCsWpfPlugIn plug-in.
    /// </summary>
    public static SampleCsWpfPlugIn Instance
    {
      get;
      private set;
    }

    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      return LoadReturnCode.Success;
    }
  }
}
