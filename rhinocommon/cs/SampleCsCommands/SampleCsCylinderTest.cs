using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;
using Rhino.Geometry;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("5afa1361-d9bb-443f-91dd-4efced6d60db")]
  public class SampleCsCylinderTest : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsCylinderTest"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select surface that look like a cylinder
      var go = new GetObject();
      go.SetCommandPrompt("Select surface that look like a cylinder");
      go.GeometryFilter = ObjectType.Surface;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var obj = go.Object(0).Object();
      var surface = go.Object(0).Surface();
      if (null == obj || null == surface)
        return Result.Failure;

      // Verify the surface looks like a cylinder
      Cylinder cylinder;
      if (!surface.TryGetCylinder(out cylinder, doc.ModelAbsoluteTolerance))
      {
        RhinoApp.WriteLine("Surface is not a cylinder");
        return Result.Success;
      }

      var circle = cylinder.CircleAt(0.0);
      var plane = circle.Plane;
      var origin = plane.Origin;

      // Calculate a plane-aligned bounding box.
      // Calculating the bounding box from the runtime object, instead
      // of a copy of the geometry, will produce a more accurate result.
      var world_to_plane = Transform.ChangeBasis(Plane.WorldXY, plane);
      var bbox = obj.Geometry.GetBoundingBox(world_to_plane);

      // Move the cylinder's plane to the base of the bounding box.
      // Create a plane through the base of the bounding box.
      var bbox_plane = new Plane(
        bbox.Corner(true, true, true),
        bbox.Corner(false, true, true),
        bbox.Corner(true, false, true)
        );
      // Transform the plane to the world xy-plane
      var plane_to_world = Transform.ChangeBasis(plane, Plane.WorldXY);
      bbox_plane.Transform(plane_to_world);
      // Project the cylinder plane's origin onto the bounding box plane
      plane.Origin = bbox_plane.ClosestPoint(origin);

      // Cylinder height is bounding box height
      var pt0 = bbox.Corner(true, true, true);
      var pt1 = bbox.Corner(true, true, false);
      var height = pt0.DistanceTo(pt1);

      // Create a new cylinder
      var new_circle = new Circle(plane, circle.Radius);
      var new_cylinder = new Cylinder(new_circle, height);
      var rev_surface = new_cylinder.ToRevSurface();
      doc.Objects.AddSurface(rev_surface);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
