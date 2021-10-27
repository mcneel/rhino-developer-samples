using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;
using Rhino.Geometry;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsCurveSeam
  /// </summary>
  public class SampleCsCurveSeam : Command
  {
    public override string EnglishName => "SampleCsCurveSeam";

    /// <summary>
    /// RunCommand
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject
      {
        GeometryFilter = ObjectType.Curve,
        GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve,
        SubObjectSelect = false,
        ReferenceObjectSelect = false
      };
      go.SetCommandPrompt("Select closed curve for seam adjustment");
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var objref = go.Object(0);
      var curve = objref.Curve();
      if (null == curve)
        return Result.Failure;

      var snapToKnots = true;

      var gp = new SampleCsCurveSeamPoint(curve, snapToKnots);
      gp.SetCommandPrompt("New seam point");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var point = gp.Point();
      if (gp.GetSeamPoint(point))
      {
        var t = gp.SeamParameter;
        var p = curve.PointAt(t);
        if (p.DistanceTo(curve.PointAtStart) > 0.00001) // some minimum distance
        {
          var new_curve = curve.DuplicateCurve();
          new_curve.ChangeClosedCurveSeam(t);
          doc.Objects.Replace(objref, new_curve);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }

  /// <summary>
  /// SampleCsCurveSeamPoint
  /// </summary>
  internal class SampleCsCurveSeamPoint : GetPoint
  {
    private readonly Curve m_curve;
    private readonly bool m_snap_to_knots;
    private double m_seam_t;

    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsCurveSeamPoint(Curve curve, bool snapToKnots)
    {
      m_curve = curve;
      m_snap_to_knots = snapToKnots;
      m_seam_t = RhinoMath.UnsetValue;
      Constrain(m_curve, false);
    }

    /// <summary>
    /// SeamParameter
    /// </summary>
    public double SeamParameter => m_seam_t;

    /// <summary>
    /// OnMouseMove
    /// </summary>
    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      GetSeamPoint(e.Point);
      base.OnMouseMove(e);
    }

    /// <summary>
    /// OnDynamicDraw
    /// </summary>
    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      e.Display.DrawPoint(m_curve.PointAtStart);
      if (RhinoMath.IsValidDouble(m_seam_t))
      {
        var p = m_curve.PointAt(m_seam_t);
        e.Display.DrawPoint(p);
        var v = m_curve.TangentAt(m_seam_t);
        e.Display.DrawDirectionArrow(p, v, Rhino.ApplicationSettings.AppearanceSettings.FeedbackColor);
      }
    }

    /// <summary>
    /// GetSeamPoint
    /// </summary>
    public bool GetSeamPoint(Point3d point)
    {
      m_seam_t = RhinoMath.UnsetValue;
      var rc = m_curve.ClosestPoint(point, out var t);
      if (rc)
      {
        if (m_snap_to_knots)
          m_seam_t = SnapToClosestKnot(t);
        else
          m_seam_t = t;
      }
      return rc;
    }

    /// <summary>
    /// SnapToClosestKnot
    /// </summary>
    private double SnapToClosestKnot(double t)
    {
      var nc = m_curve as NurbsCurve;
      if (null == nc)
        return t;

      var kcount = nc.Knots.Count;
      for (int i = 1; i < kcount; i++)
      {
        var k0 = nc.Knots[i - 1];
        var k1 = nc.Knots[i];

        if (Math.Abs(k0 - t) < RhinoMath.ZeroTolerance)
          return k0;
        if (Math.Abs(k1 - t) < RhinoMath.ZeroTolerance)
          return k1;

        if (k0 < t && k1 > t)
        {
          var p = nc.PointAt(t);
          var p0 = nc.PointAt(k0);
          var p1 = nc.PointAt(k1);
          if (p.DistanceTo(p0) > p.DistanceTo(p1))
            return k1;
          else
            return k0;
        }
      }
      return t;
    }
  }
}
