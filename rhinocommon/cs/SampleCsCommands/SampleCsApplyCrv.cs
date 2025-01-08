using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;
using System.Collections.Generic;

namespace SampleCsCommands
{
  public class SampleCsApplyCrv : Command
  {
    public override string EnglishName => "SampleCsApplyCrv";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject gc = new GetObject();
      gc.SetCommandPrompt("Select curves on world XY plane to apply to a surface");
      gc.GeometryFilter = ObjectType.Curve;
      gc.EnablePreSelect(true, true);
      gc.GetMultiple(1, 0);
      if (gc.CommandResult() != Result.Success)
        return gc.CommandResult();

      double tol = doc.ModelAbsoluteTolerance;
      BoundingBox bbox = new BoundingBox();

      List<Curve> input_curves = new List<Curve>(gc.ObjectCount);
      foreach (ObjRef curve_ref in gc.Objects())
      {
        Curve curve = curve_ref.Curve();
        if (null == curve)
          continue;
        if (!curve.IsValid)
          continue;
        if (!curve.IsPlanar(tol))
          continue;
        if (!curve.IsInPlane(Plane.WorldXY, tol))
          continue;
        bbox.Union(curve.GetBoundingBox(true));
        input_curves.Add(curve);
      }

      if (0 == input_curves.Count)
        return Result.Failure;

      GetObject gs = new GetObject();
      gs.SetCommandPrompt("Select surface to apply the planar curves to");
      gs.GeometryFilter = ObjectType.Surface;
      gs.EnablePreSelect(false, true);
      gs.DeselectAllBeforePostSelect = false;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      BrepFace brep_face = gs.Object(0).Face();
      Surface surface = brep_face?.UnderlyingSurface();
      if (surface == null)
        return Result.Failure;

      Interval srf_u_domain = surface.Domain(0);
      Interval srf_v_domain = surface.Domain(1);
      double srf_u_extent = srf_u_domain.Max - srf_u_domain.Min;
      double srf_v_extent = srf_v_domain.Max - srf_v_domain.Min;

      Interval bbox_x_domain = new Interval(bbox.Min.X, bbox.Max.X);
      Interval bbox_y_domain = new Interval(bbox.Min.Y, bbox.Max.Y);
      double bbox_x_extent = bbox_x_domain.Max - bbox_x_domain.Min;
      double bbox_y_extent = bbox_y_domain.Max - bbox_y_domain.Min;
      if (bbox_x_extent <= 0.0 || bbox_y_extent <= 0.0)
        return Result.Failure;

      double scale0 = srf_u_extent / bbox_x_extent;
      double scale1 = srf_v_extent / bbox_y_extent;

      Transform xform = new Transform
      {
        M00 = scale0,
        M11 = scale1,
        M22 = 1,
        M33 = 1,
        M03 = srf_u_domain.Min - bbox_x_domain.Min * scale0,
        M13 = srf_v_domain.Min - bbox_y_domain.Min * scale1
      };

      foreach (Curve curve in input_curves)
      {
        Curve curve2d = curve.DuplicateCurve();
        if (null != curve2d)
        {
          curve2d.Transform(xform);
          Curve curve3d = surface.Pushup(curve2d, tol);
          if (null != curve3d)
            doc.Objects.AddCurve(curve3d);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
