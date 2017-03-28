using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("ffb318f1-8836-416f-9aef-5b99efb4558c")]
  public class SampleCsStackedControlPointsCurve : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsStackedControlPointsCurve"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef object_ref;
      var rc = RhinoGet.GetOneObject("Select curve for stacked control point test", false, ObjectType.Curve, out object_ref);
      if (rc != Result.Success)
        return rc;

      var nc = object_ref.Curve() as NurbsCurve;
      if (null == nc)
      {
        RhinoApp.WriteLine("Curve is not a NURBS curve.");
        return Result.Success;
      }

      var b = IsStackedControlPointsCurve(nc);
      if (b)
        RhinoApp.WriteLine("NURBS curve has stacked control points.");
      else
        RhinoApp.WriteLine("NURBS curve does not have stacked control points.");

      return Result.Success;
    }

    public static bool IsStackedControlPointsCurve(NurbsCurve nc)
    {
      var rc = false;
      if (null != nc && nc.IsValid)
      {
        for (var i = 0; i < nc.Points.Count - 1; i++)
        {
          var cv0 = nc.Points[i];
          var cv1 = nc.Points[i + 1];
          var dist = cv0.Location.DistanceTo(cv1.Location);
          if (dist < RhinoMath.ZeroTolerance)
          {
            rc = true;
            break;
          }
        }
      }
      return rc;
    }
  }
}
