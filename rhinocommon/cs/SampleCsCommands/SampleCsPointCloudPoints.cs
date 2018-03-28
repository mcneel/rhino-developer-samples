using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsPointCloudPoints : Command
  {
    public SampleCsPointCloudPoints()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsPointCloudPoints"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select point cloud");
      go.GeometryFilter = ObjectType.PointSet;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var point_cloud_object = go.Object(0).Object() as PointCloudObject;
      if (null == point_cloud_object)
        return Result.Failure;

      var gp = new GetPointCloudPoints(point_cloud_object.Id);
      gp.SetCommandPrompt("Select point cloud points");
      gp.GetMultiple(1, 0);
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      foreach (var objref in gp.Objects())
      {
        var ci = objref.GeometryComponentIndex;
        var pt = point_cloud_object.PointCloudGeometry[ci.Index].Location;
        RhinoApp.WriteLine(string.Format("Index {0}: {1}", ci.Index, pt.ToString()));
      }

      return Result.Success;
    }
  }

  /// <summary>
  /// GetPointCloudPoints
  /// </summary>
  class GetPointCloudPoints : GetObject
  {
    private readonly Guid m_object_id;

    public GetPointCloudPoints(Guid objectId)
    {
      m_object_id = objectId;
      GeometryFilter = ObjectType.Point;
      SubObjectSelect = true;
    }

    public override bool CustomGeometryFilter(RhinoObject obj, GeometryBase geom, ComponentIndex ci)
    {
      return (null != obj && obj.Id.Equals(m_object_id));
    }
  }

}
