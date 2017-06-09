using Rhino.ApplicationSettings;
using Rhino.Display;
using Rhino.Geometry;

namespace SampleCsMobilePlane
{
  /// <summary>
  /// SampleCsMobilePlaneConduit
  /// </summary>
  public class SampleCsMobilePlaneConduit : DisplayConduit
  {
    public Plane Plane { get; private set; }

    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsMobilePlaneConduit(Plane plane)
    {
      Plane = plane;
      Enabled = true;
    }

    /// <summary>
    /// Draw direction arrows using the plane member.
    /// </summary>
    /// <param name="e"></param>
    protected override void DrawOverlay(DrawEventArgs e)
    {
      e.Display.DrawDirectionArrow(Plane.Origin, Plane.XAxis, AppearanceSettings.GridXAxisLineColor);
      e.Display.DrawDirectionArrow(Plane.Origin, Plane.YAxis, AppearanceSettings.GridYAxisLineColor);
      e.Display.DrawDirectionArrow(Plane.Origin, Plane.ZAxis, AppearanceSettings.GridZAxisLineColor);
    }
  }
}
