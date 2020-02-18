using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsTestPlanarCurveContainment : Command
  {
    public override string EnglishName => "SampleCsTestPlanarCurveContainment";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select two curves for planar curve containment test");
      go.GeometryFilter = ObjectType.Curve;
      go.GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve;
      go.SubObjectSelect = false;
      go.GetMultiple(2, 2);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var curve_a = go.Object(0).Curve();
      var curve_b = go.Object(1).Curve();
      if (null == curve_a || null == curve_b)
        return Result.Failure;

      var tol = doc.ModelAbsoluteTolerance;


      if (!curve_a.IsPlanar(tol) || !curve_a.TryGetPlane(out var plane_a, tol))
      {
        RhinoApp.WriteLine("The first curve is not planar.");
        return Result.Success;
      }

      if (!curve_b.IsPlanar(tol) || !curve_b.TryGetPlane(out var plane_b, tol))
      {
        RhinoApp.WriteLine("The second curve is not planar.");
        return Result.Success;
      }

      if (!curve_a.IsInPlane(plane_b, tol) || !curve_b.IsInPlane(plane_a, tol))
      {
        RhinoApp.WriteLine("The two curves are not co-planar.");
        return Result.Success;
      }

      var rc = Curve.PlanarClosedCurveRelationship(curve_a, curve_b, plane_a, tol);
      switch (rc)
      {
        case RegionContainment.Disjoint:
          RhinoApp.WriteLine("There is no common area between the two curve regions.");
          break;
        case RegionContainment.MutualIntersection:
          RhinoApp.WriteLine("The two curves intersect. No full containment relationship exists.");
          break;
        case RegionContainment.AInsideB:
          RhinoApp.WriteLine("The region bounded by the first curve is inside of the second curve.");
          break;
        case RegionContainment.BInsideA:
          RhinoApp.WriteLine("The region bounded by the second curve is inside of the first curve.");
          break;
      }

      return Result.Success;
    }
  }
}
