using Rhino.DocObjects;
using Rhino.Geometry;

namespace SampleCsRectangleGrips
{
  internal class SampleCsRectangleGripsEnabler
  {
    /// <summary>
    /// Delegate function used to turn custom grips on
    /// </summary>
    public void TurnOnGrips(RhinoObject rhObject)
    {
      if (rhObject == null)
        return;

      var polyline_curve = rhObject.Geometry as PolylineCurve;
      if (polyline_curve == null)
        return;

      if (!SampleCsRectangleHelper.IsRectangle(polyline_curve))
        return;

      var rectangle_grips = new SampleCsRectangleGrips();
      if (!rectangle_grips.CreateGrips(polyline_curve))
        return;

      rhObject.EnableCustomGrips(rectangle_grips);
    }
  }
}
