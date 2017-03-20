using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("6b55aabc-fce8-4f21-9793-27ecb7692aaa")]
  public class SampleCsFairCurves : Command
  {
    private enum FairClamp
    {
      None = 0,
      Tangency = 1,
      Curvature = 2
    }

    private double m_dist_tol;
    private readonly double m_ang_tol;
    private readonly int m_iterations;

    public SampleCsFairCurves()
    {
      m_dist_tol = 1.0;
      m_ang_tol = 0.05;
      m_iterations = 20; // Use 20 iterations as part of getting Fair to stay within stated tolerance
    }

    public override string EnglishName
    {
      get { return "SampleCsFairCurves"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select curves to fair");
      go.GeometryFilter = ObjectType.Curve;
      go.GroupSelect = true;
      go.ReferenceObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (var objref in go.Objects())
      {
        var curve = objref.Curve();
        if (null == curve)
          return Result.Failure;

        var nurb = curve.ToNurbsCurve();
        if (null == nurb)
          return Result.Failure;

        if (1 == nurb.Degree)
        {
          RhinoApp.WriteLine("Cannot fair degree 1 curves.");
          var obj = objref.Object();
          if (null != obj)
          {
            obj.Select(false);
            obj.Highlight(false);
            doc.Views.Redraw();
          }
          return Result.Cancel;
        }
      }

      var rc = GetTolerance(ref m_dist_tol);
      if (rc != Result.Success)
        return rc;

      // Divide tolerance by 4 as part of getting Fair to stay within stated tolerance
      var dist_tol = m_dist_tol * 0.25;
      var faired_count = 0;
      var degree3_count = 0;

      foreach (var objref in go.Objects())
      {
        var curve = objref.Curve();
        if (null == curve)
          return Result.Failure;

        var nurb = curve.ToNurbsCurve();
        if (null == nurb)
          return Result.Failure;

        var curve_degree = nurb.Degree;

        var fair = nurb.Fair(dist_tol, m_ang_tol, (int)FairClamp.None, (int)FairClamp.None, m_iterations);
        if (null != fair && fair.IsValid)
        {
          if (curve_degree != fair.Degree)
            degree3_count++;

          if (doc.Objects.Replace(objref, fair))
            faired_count++;
        }
      }

      if (0 == faired_count)
        RhinoApp.WriteLine("No curves were faired.");
      else if (1 == faired_count)
        RhinoApp.WriteLine("1 curve was faired.");
      else
        RhinoApp.WriteLine("{0} curves were faired.", faired_count);

      if (degree3_count > 0)
      {
        if (1 == degree3_count)
          RhinoApp.WriteLine("1 curve was changed to degree 3 during fairing.");
        else
          RhinoApp.WriteLine("{0} curves were changed to degree 3 during fairing.", degree3_count);
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    private static Result GetTolerance(ref double tolerance)
    {
      var gp = new GetPoint();
      gp.SetCommandPrompt("Tolerance");
      gp.SetDefaultNumber(tolerance);
      gp.AcceptNumber(true, false);

      for (; ; )
      {
        var res = gp.Get();

        if (res == GetResult.Number)
        {
          var d = gp.Number();
          if (d < 0.0)
          {
            RhinoApp.WriteLine("Tolerance must be greater than 0.");
          }
          else
          {
            tolerance = d;
            return Result.Success;
          }
        }

        if (res != GetResult.Point)
          return Result.Cancel;

        break;
      }

      var base_point = gp.Point();

      gp.SetBasePoint(base_point, true);
      gp.DrawLineFromPoint(base_point, true);
      gp.DynamicDrawColor = Rhino.ApplicationSettings.AppearanceSettings.TrackingColor;

      for (; ; )
      {
        var res = gp.Get();

        if (res == GetResult.Number)
        {
          var d = gp.Number();
          if (d < 0.0)
          {
            RhinoApp.WriteLine("Tolerance must be greater than 0.");
          }
          else
          {
            tolerance = d;
            return Result.Success;
          }
        }

        if (res != GetResult.Point)
          return Result.Cancel;

        break;
      }

      tolerance = base_point.DistanceTo(gp.Point());
      return Result.Success;
    }

  }
}
