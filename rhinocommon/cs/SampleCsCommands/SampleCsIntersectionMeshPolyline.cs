using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("901bed9e-b2c9-47f3-9e43-53da8eda682f")]
  public class SampleCsIntersectionMeshPolyline : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsIntersectionMeshPolyline"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gm = new GetObject();
      gm.SetCommandPrompt("Select mesh");
      gm.GeometryFilter = ObjectType.Mesh;
      gm.Get();
      if (gm.CommandResult() != Result.Success)
        return gm.CommandResult();

      var gp = new GetObject();
      gp.SetCommandPrompt("Select polyline");
      gp.GeometryFilter = ObjectType.Curve;
      gp.EnablePreSelect(false, true);
      gp.DeselectAllBeforePostSelect = false;
      gp.Get();
      if (gm.CommandResult() != Result.Success)
        return gp.CommandResult();

      var mesh = gm.Object(0).Mesh();
      if (null == mesh)
        return Result.Failure;

      var curve = gp.Object(0).Curve();
      if (null == curve)
        return Result.Failure;

      var polyline_curve = curve as PolylineCurve;
      if (null == polyline_curve)
      {
        RhinoApp.Write("Curve is not a polyline");
        return Result.Nothing;
      }

      int[] face_ids;
      var points = Rhino.Geometry.Intersect.Intersection.MeshPolyline(mesh, polyline_curve, out face_ids);
      if (points.Length > 0)
      {
        foreach (var pt in points)
          doc.Objects.AddPoint(pt);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
