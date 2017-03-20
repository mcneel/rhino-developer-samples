using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("83352b46-7061-41dc-b7f9-bd83e647d4dd")]
  public class SampleCsProjectPointToMesh : Command
  {
    public SampleCsProjectPointToMesh()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsProjectPointToMesh"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gp = new GetObject();
      gp.SetCommandPrompt("Select points to project");
      gp.GeometryFilter = ObjectType.Point;
      gp.GetMultiple(1, 0);
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var gm = new GetObject();
      gm.SetCommandPrompt("Select mesh to project onto");
      gm.GeometryFilter = ObjectType.Mesh;
      gm.SubObjectSelect = false;
      gm.EnablePreSelect(false, true);
      gm.DeselectAllBeforePostSelect = false;
      gm.Get();
      if (gm.CommandResult() != Result.Success)
        return gm.CommandResult();

      var mesh = gm.Object(0).Mesh();
      if (null == mesh)
        return Result.Failure;

      var meshes = new List<Mesh>(1) {mesh};

      var points = new List<Point3d>(gp.ObjectCount);
      for (var i = 0; i < gp.ObjectCount; i++)
      {
        var point = gp.Object(i).Point();
        if (null != point) 
          points.Add(point.Location);
      }

      var dir = -Vector3d.ZAxis;
      var tol = doc.ModelAbsoluteTolerance;

      int[] indices;
      var project_points = Intersection.ProjectPointsToMeshesEx(meshes, points, dir, tol, out indices);
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
