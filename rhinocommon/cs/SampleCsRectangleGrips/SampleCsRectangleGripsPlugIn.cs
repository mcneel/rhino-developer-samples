using Rhino.PlugIns;

namespace SampleCsRectangleGrips
{
  public class SampleCsRectangleGripsPlugIn : PlugIn
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsRectangleGripsPlugIn()
    {
      Instance = this;
    }

    /// <summary>
    /// Gets the only and only instance of the SampleCsRectangleGripsPlugIn plug-in
    /// </summary>
    public static SampleCsRectangleGripsPlugIn Instance { get; private set; }
  }
}