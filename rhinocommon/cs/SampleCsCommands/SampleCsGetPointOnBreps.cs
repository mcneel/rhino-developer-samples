using System;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;
using Rhino;
using Rhino.ApplicationSettings;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsGetPointOnBreps
  /// </summary>
  public class SampleCsGetPointOnBreps : Command
  {
    public SampleCsGetPointOnBreps()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsGetPointOnBreps"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select surfaces and polysurfaces");
      go.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var gp = new GetPointOnBreps();
      gp.SetCommandPrompt("Point on surface or polysurface");

      foreach (var obj_ref in go.Objects())
        gp.Breps.Add(obj_ref.Brep());

      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var point = gp.Point();

      // One final calculation
      var closest_point = gp.CalculateClosestPoint(point);
      if (closest_point.IsValid)
      {
        doc.Objects.AddPoint(closest_point);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }


  /// <summary>
  /// GetPointOnBreps
  /// </summary>
  class GetPointOnBreps : GetPoint
  {
    public readonly List<Brep> Breps;
    public Point3d ClosestPoint;

    public GetPointOnBreps()
    {
      Breps = new List<Brep>();
      ClosestPoint = Point3d.Unset;
    }

    public Point3d CalculateClosestPoint(Point3d point)
    {
      var closest_point = Point3d.Unset;
      var minimum_distance = Double.MaxValue;
      foreach (var brep in Breps)
      {
        foreach (var face in brep.Faces)
        {
          double u, v;
          if (face.ClosestPoint(point, out u, out v))
          {
            var face_point = face.PointAt(u, v);
            double distance = face_point.DistanceTo(point);
            if (distance < minimum_distance)
            {
              minimum_distance = distance;
              closest_point = face_point;
            }
          }
        }
      }
      return closest_point;
    }

    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      ClosestPoint = CalculateClosestPoint(e.Point);
      base.OnMouseMove(e);
    }

    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      if (ClosestPoint.IsValid)
        e.Display.DrawPoint(ClosestPoint, AppearanceSettings.DefaultObjectColor);

      // Do not call base class...
      //base.OnDynamicDraw(e);
    }
  }
}
