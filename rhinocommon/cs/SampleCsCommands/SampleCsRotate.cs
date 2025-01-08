using Rhino;
using Rhino.ApplicationSettings;
using Rhino.Collections;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using System;

namespace SampleCsCommands
{
  #region GetReferencePoint class

  /// <summary>
  /// GetReferencePoint class
  /// </summary>
  internal class GetReferencePoint : GetPoint
  {
    private readonly Point3d m_base_point;
    private Circle m_circle;

    public GetReferencePoint(Point3d basePoint)
    {
      m_base_point = basePoint;
      AcceptNumber(true, false);
      SetBasePoint(m_base_point, true);
      DrawLineFromPoint(m_base_point, true);
      ConstrainToConstructionPlane(true);
    }

    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      m_circle = new Circle(e.Viewport.ConstructionPlane(), m_base_point, e.Point.DistanceTo(m_base_point));
      base.OnMouseMove(e);
    }

    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      if (m_circle.IsValid)
        e.Display.DrawCircle(m_circle, AppearanceSettings.TrackingColor);
      base.OnDynamicDraw(e);
    }
  }

  #endregion


  #region GetRotationTransform class

  /// <summary>
  /// GetRotationTransform
  /// </summary>
  internal class GetRotationTransform : GetTransform
  {
    private Plane m_plane;
    private readonly Point3d m_base_point;
    private readonly Point3d m_ref_point;
    private double m_angle;
    private Arc m_arc;

    public GetRotationTransform(Plane plane, Point3d basePoint, Point3d refPoint)
    {
      m_angle = 0.0;
      m_plane = plane;
      m_base_point = basePoint;
      m_ref_point = refPoint;
      AcceptNumber(true, false);
      SetBasePoint(m_base_point, true);
      Constrain(m_plane, false);
      ConstrainDistanceFromBasePoint(m_base_point.DistanceTo(m_ref_point));
    }

    public double Angle => m_angle;

    public override Transform CalculateTransform(RhinoViewport viewport, Point3d point)
    {
      if (CalculatePlaneAngle(point))
        return Transform.Rotation(m_angle, m_plane.Normal, m_base_point);
      return Transform.Identity;
    }

    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      if (Transform.IsValid && m_arc.IsValid)
      {
        System.Drawing.Color color = Rhino.ApplicationSettings.AppearanceSettings.DefaultObjectColor;
        e.Display.DrawArc(m_arc, color);

        Vector3d v0 = m_arc.StartPoint - m_arc.Center;
        v0 *= 1.5;
        e.Display.DrawLine(m_arc.Center, m_arc.Center + v0, color);

        v0 = m_arc.EndPoint - m_arc.Center;
        v0 *= 1.5;
        Vector3d v1 = (e.CurrentPoint - m_arc.Center);
        if (v1.SquareLength > v0.SquareLength)
          v0 = v1;
        e.Display.DrawLine(m_arc.Center, m_arc.Center + v0, color);

        if (m_angle < 0.0)
          e.Display.DrawPoint(m_arc.EndPoint, color);
        else
          e.Display.DrawPoint(m_arc.StartPoint, color);
        e.Display.DrawPoint(m_arc.Center, color);

        Rhino.UI.StatusBar.SetPointPane(e.CurrentPoint);
        Rhino.UI.StatusBar.SetDistancePane(m_angle * 180.0 / Math.PI);
      }

      base.OnDynamicDraw(e);
    }

    private bool CalculatePlaneAngle(Point3d point)
    {
      Point3d plane_point = m_plane.ClosestPoint(point);

      Vector3d dir0 = plane_point - m_plane.Origin;
      double cos_angle = dir0 * m_plane.XAxis;
      double sin_angle = dir0 * m_plane.YAxis;
      dir0.Unitize();

      if (0.0 != cos_angle || 0.0 != sin_angle)
      {
        double angle = Math.Atan2(sin_angle, cos_angle);
        double a0 = Math.IEEERemainder(0.5 * m_angle / Math.PI, 1.0);
        double a1 = Math.IEEERemainder(0.5 * angle / Math.PI, 1.0);
        if (a0 != a1)
        {
          double da = a1 - a0;
          while (da < -0.5)
            da += 1.0;
          while (da > 0.5)
            da -= 1.0;
          m_angle += da * 2.0 * Math.PI;

          const double two_pi = 2.0 * Math.PI;
          if (m_angle < -two_pi)
            m_angle += two_pi;
          else if (m_angle > two_pi)
            m_angle -= two_pi;
        }

        Vector3d dir1 = m_ref_point - m_base_point;
        double radius = dir1.Length;
        dir1.Unitize();

        Vector3d x_dir = m_angle < 0.0 ? dir0 : dir1;
        Vector3d y_dir = Vector3d.CrossProduct(m_plane.Normal, x_dir);
        y_dir.Unitize();

        Plane arc_plane = new Plane(m_plane.Origin, x_dir, y_dir);
        m_arc = new Arc(arc_plane, radius, Math.Abs(m_angle));

        return m_arc.IsValid;
      }

      return false;
    }
  }

  #endregion


  #region SampleCsRotate command

  /// <summary>
  /// SampleCsRotate command
  /// </summary>
  public class SampleCsRotate : TransformCommand
  {
    public override string EnglishName => "SampleCsRotate";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select objects to rotate
      TransformObjectList list = new TransformObjectList();
      Result rc = SelectObjects("Select objects to rotate", list);
      if (rc != Result.Success)
        return rc;

      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Center of rotation");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      RhinoView view = gp.View();
      if (null == view)
        return Result.Failure;

      Point3d base_point = gp.Point();
      Plane plane = view.ActiveViewport.ConstructionPlane();
      plane.Origin = base_point;

      Point3d ref_point = Point3d.Unset;


      // Angle or first reference point
      GetReferencePoint gr = new GetReferencePoint(base_point);
      gr.SetCommandPrompt("Angle or first reference point");
      GetResult res = gr.Get();
      if (res == GetResult.Point)
      {
        view = gr.View();
        rc = null != view ? Result.Success : Result.Failure;
        if (rc == Result.Success)
        {
          plane = view.ActiveViewport.ConstructionPlane();
          plane.Origin = base_point;
          ref_point = plane.ClosestPoint(gr.Point());
          if (base_point.DistanceTo(ref_point) <= RhinoMath.ZeroTolerance)
            rc = Result.Nothing;
        }
        if (rc != Result.Success)
          return rc;
      }
      else if (res == GetResult.Number)
      {
        Transform xform = Transform.Rotation(Rhino.RhinoMath.ToRadians(gr.Number()), plane.Normal, base_point);
        rc = xform.IsValid ? Result.Success : Result.Failure;
        if (rc == Result.Success)
        {
          TransformObjects(list, xform, false, false);
          doc.Views.Redraw();
        }
        return rc;
      }
      else
      {
        return Result.Cancel;
      }


      // Second reference point
      GetRotationTransform gx = new GetRotationTransform(plane, base_point, ref_point);
      gx.SetCommandPrompt("Second reference point");
      gx.AddTransformObjects(list);
      res = gx.GetXform();
      if (res == GetResult.Point)
      {
        view = gx.View();
        rc = null != view ? Result.Success : Result.Failure;
        if (rc == Result.Success)
        {
          Transform xform = gx.CalculateTransform(view.ActiveViewport, gx.Point());
          rc = xform.IsValid ? Result.Success : Result.Failure;
          if (rc == Result.Success)
          {
            TransformObjects(list, xform, false, false);
            doc.Views.Redraw();
          }
        }
      }
      else if (res == GetResult.Number)
      {
        Transform xform = Transform.Rotation(Rhino.RhinoMath.ToRadians(gx.Number()), plane.Normal, base_point);
        rc = xform.IsValid ? Result.Success : Result.Failure;
        if (rc == Result.Success)
        {
          TransformObjects(list, xform, false, false);
          doc.Views.Redraw();
        }
      }
      else
      {
        rc = Result.Cancel;
      }

      return rc;
    }
  }

  #endregion
}
