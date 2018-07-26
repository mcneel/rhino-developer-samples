using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsMake2D : Command
  {
    public override string EnglishName => "SampleCsMake2d";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select objects to test Make2D Points");
      go.GeometryFilter = ObjectType.Point | ObjectType.Surface | ObjectType.PolysrfFilter;
      go.GroupSelect = true;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      var obj_refs = go.Objects();

      var hld_params = new HiddenLineDrawingParameters
      {
        AbsoluteTolerance = doc.ModelAbsoluteTolerance,
        IncludeTangentEdges = false,
        IncludeHiddenCurves = true
      };

      hld_params.SetViewport(view.ActiveViewport);

      foreach (var obj_ref in obj_refs)
      {
        var obj = obj_ref?.Object();
        if (obj != null)
          hld_params.AddGeometry(obj.Geometry, Transform.Identity, obj.Id);
      }

      var hld = HiddenLineDrawing.Compute(hld_params, true);

      if (hld != null)
      {
        var flatten = Transform.PlanarProjection(Plane.WorldXY);
        BoundingBox page_box = hld.BoundingBox(true);
        var delta_2d = new Vector2d(0, 0);
        delta_2d = delta_2d - new Vector2d(page_box.Min.X, page_box.Min.Y);
        var delta_3d = Transform.Translation(new Vector3d(delta_2d.X, delta_2d.Y, 0.0));
        flatten = delta_3d * flatten;

        var h_attribs = new ObjectAttributes { Name = "H" };
        var v_attribs = new ObjectAttributes { Name = "V" };

        foreach (var hld_curve in hld.Segments)
        {
          if (hld_curve?.ParentCurve == null || hld_curve.ParentCurve.SilhouetteType == SilhouetteType.None)
            continue;

          var crv = hld_curve.CurveGeometry.DuplicateCurve();
          if (crv != null)
          {
            crv.Transform(flatten);
            switch (hld_curve.SegmentVisibility)
            {
              case HiddenLineDrawingSegment.Visibility.Visible:
                doc.Objects.AddCurve(crv, v_attribs);
                break;
              case HiddenLineDrawingSegment.Visibility.Hidden:
                doc.Objects.AddCurve(crv, h_attribs);
                break;
            }
          }
        }

        foreach (var hld_pt in hld.Points)
        {
          if (hld_pt == null)
            continue;

          var pt = hld_pt.Location;
          if (pt.IsValid)
          {
            pt.Transform(flatten);
            switch (hld_pt.PointVisibility)
            {
              case HiddenLineDrawingPoint.Visibility.Visible:
                doc.Objects.AddPoint(pt, v_attribs);
                break;
              case HiddenLineDrawingPoint.Visibility.Hidden:
                doc.Objects.AddPoint(pt, h_attribs);
                break;
            }
          }
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}