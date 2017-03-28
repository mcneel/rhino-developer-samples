using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsRectangleGrips
{
  internal class SampleCsGetRectangleCurve : GetObject
  {
    /// <summary>
    /// Override to provide fancy object filtering
    /// </summary>
    public override bool CustomGeometryFilter(RhinoObject rhObject, GeometryBase geometry, ComponentIndex componentIndex)
    {
      return SampleCsRectangleHelper.IsRectangle(geometry);
    }
  }
}
