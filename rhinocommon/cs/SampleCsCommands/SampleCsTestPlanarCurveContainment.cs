using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsTestPlanarCurveContainment : Command
  {
    public SampleCsTestPlanarCurveContainment()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsTestPlanarCurveContainment"; }
    }

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

      Rhino.Geometry.Curve curveA = go.Object(0).Curve();
      Rhino.Geometry.Curve curveB = go.Object(1).Curve();
      if (null == curveA || null == curveB)
        return Result.Failure;

      Plane planeA, planeB;
      if (!curveA.IsPlanar() || !curveA.TryGetPlane(out planeA))
      {
        RhinoApp.WriteLine("Curve A is not planar.");
        return Result.Success;
      }

      if (!curveB.IsPlanar() || !curveB.TryGetPlane(out planeB))
      {
        RhinoApp.WriteLine("Curve B is not planar.");
        return Result.Success;
      }

      double tol = Rhino.RhinoMath.ZeroTolerance;
      RegionContainment rc = Curve.PlanarClosedCurveRelationship(curveA, curveB, planeA, tol);
      switch (rc)
      {
        case RegionContainment.Disjoint:
          RhinoApp.WriteLine("There is no common area between the two regions.");
          break;
        case RegionContainment.MutualIntersection:
          RhinoApp.WriteLine("The two curves intersect. No full containment relationship exists.");
          break;
        case RegionContainment.AInsideB:
          RhinoApp.WriteLine("Region bounded by curveA (first curve) is inside of curveB (second curve).");
          break;
        case RegionContainment.BInsideA:
          RhinoApp.WriteLine("Region bounded by curveB (second curve) is inside of curveA (first curve).");
          break;
      }

      return Result.Success;
    }
  }
}
