using Rhino.PlugIns;

namespace SampleCsEventWatcher
{
  /// <summary>
  /// SampleCsEventWatcherPlugIn plug-in
  /// </summary>
  public class SampleCsEventWatcherPlugIn : PlugIn
  {
    public SampleCsEventWatcherPlugIn()
    {
      // Rhino only creates one instance of each plug-in class,
      // so it is safe to store a reference in a static property.
      Instance = this;
    }

    /// <summary>
    /// The one and only instance of this plug-in.
    /// </summary>
    public static SampleCsEventWatcherPlugIn Instance
    {
      get;
      private set;
    }

    /// <summary>
    /// Called by Rhino when loading this plug-in.
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      SampleCsEventHandlers.Instance.Enable(true);
      return LoadReturnCode.Success;
    }
  }
}