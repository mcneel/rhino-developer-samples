using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsPlanarClosedCurveRelationship : Command
  {
    public SampleCsPlanarClosedCurveRelationship()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsPlanarClosedCurveRelationship"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select closed, planar curves for containment test");
      go.GeometryFilter = ObjectType.Curve;
      go.GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve;
      go.GetMultiple(2, 2);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var plane = go.View().ActiveViewport.ConstructionPlane();
      var tol = doc.ModelAbsoluteTolerance;

      var crvs = new Curve[2];
      for (var i = 0; i < 2; i++)
      {
        var rh_ref = go.Object(i);
        var rh_obj = rh_ref.Object();
        crvs[i] = rh_ref.Curve();
        if (null == rh_obj || null == crvs[i])
          return Result.Failure;

        if (!crvs[i].IsPlanar(tol))
        {
          RhinoApp.WriteLine("Curve is not planar");
          rh_obj.Select(true);
          doc.Views.Redraw();
          return Result.Failure;
        }
      }

      var result = Curve.PlanarClosedCurveRelationship(crvs[0], crvs[1], plane, tol);
      switch (result)
      {
        case RegionContainment.Disjoint:
          RhinoApp.WriteLine("There is no common area between the two regions.");
          break;
        case RegionContainment.MutualIntersection:
          RhinoApp.WriteLine("The two curves intersect. There is therefore no full containment relationship either way.");
          break;
        case RegionContainment.AInsideB:
          RhinoApp.WriteLine("The region bounded by the first curve is inside of the second curve.");
          break;
        case RegionContainment.BInsideA:
          RhinoApp.WriteLine("The region bounded by the second curve is inside of the second first.");
          break;
      }

      return Result.Success;
    }
  }
}
