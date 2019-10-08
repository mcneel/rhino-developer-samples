using Rhino.PlugIns;

namespace SampleSharedXformOwner
{
  /// <summary>
  /// The SampleSharedXformOwnerPlugIn plug-in class
  /// </summary>
  public class SampleSharedXformOwnerPlugIn : PlugIn
  {
    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleSharedXformOwnerPlugIn()
    {
      Instance = this;
    }

    /// <summary>
    /// Gets the only instance of the SampleSharedXformOwnerPlugIn plug-in.
    /// </summary>
    public static SampleSharedXformOwnerPlugIn Instance { get; private set; }

    /// <summary>
    /// Called when the plug-in is being loaded.
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      return LoadReturnCode.Success;
    }
  }
}