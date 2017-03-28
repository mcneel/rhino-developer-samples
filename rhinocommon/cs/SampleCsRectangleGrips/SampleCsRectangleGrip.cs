using Rhino.DocObjects.Custom;

namespace SampleCsRectangleGrips
{
  public class SampleCsRectangleGrip : CustomGripObject
  {
    /// <summary>
    /// True if grip motion can change dimension
    /// </summary>
    public bool Active { get; set; }

    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsRectangleGrip()
    {
      Active = true;
    }

    /// <summary>
    /// RhinoObject override
    /// </summary>
    public override string ShortDescription(bool plural)
    {
      return plural ? "rectangle points" : "rectangle point";
    }
  }
}
