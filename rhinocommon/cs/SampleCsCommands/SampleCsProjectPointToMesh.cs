using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;
using System.Collections.Generic;

namespace SampleCsCommands
{
  public class SampleCsProjectPointToMesh : Command
  {
    public override string EnglishName => "SampleCsProjectPointToMesh";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject gp = new GetObject();
      gp.SetCommandPrompt("Select points to project");
      gp.GeometryFilter = ObjectType.Point;
      gp.GetMultiple(1, 0);
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      GetObject gm = new GetObject();
      gm.SetCommandPrompt("Select mesh to project onto");
      gm.GeometryFilter = ObjectType.Mesh;
      gm.SubObjectSelect = false;
      gm.EnablePreSelect(false, true);
      gm.DeselectAllBeforePostSelect = false;
      gm.Get();
      if (gm.CommandResult() != Result.Success)
        return gm.CommandResult();

      Mesh mesh = gm.Object(0).Mesh();
      if (null == mesh)
        return Result.Failure;

      List<Mesh> meshes = new List<Mesh>(1) { mesh };

      List<Point3d> points = new List<Point3d>(gp.ObjectCount);
      for (int i = 0; i < gp.ObjectCount; i++)
      {
        Point point = gp.Object(i).Point();
        if (null != point)
          points.Add(point.Location);
      }

      Vector3d dir = -Vector3d.ZAxis;
      double tol = doc.ModelAbsoluteTolerance;

      int[] indices;
      Point3d[] project_points = Intersection.ProjectPointsToMeshesEx(meshes, points, dir, tol, out indices);
      if (null != project_points)
      {
        for (int i = 0; i < project_points.Length; i++)
        {
          doc.Objects.AddPoint(project_points[i]);
          doc.Objects.AddLine(project_points[i], points[indices[i]]);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
