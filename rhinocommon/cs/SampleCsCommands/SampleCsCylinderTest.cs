using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsCylinderTest : Command
  {
    public override string EnglishName => "SampleCsCylinderTest";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select surface that look like a cylinder
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select surface that look like a cylinder");
      go.GeometryFilter = ObjectType.Surface;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      RhinoObject obj = go.Object(0).Object();
      Surface surface = go.Object(0).Surface();
      if (null == obj || null == surface)
        return Result.Failure;

      // Verify the surface looks like a cylinder
      Cylinder cylinder;
      if (!surface.TryGetCylinder(out cylinder, doc.ModelAbsoluteTolerance))
      {
        RhinoApp.WriteLine("Surface is not a cylinder");
        return Result.Success;
      }

      Circle circle = cylinder.CircleAt(0.0);
      Plane plane = circle.Plane;
      Point3d origin = plane.Origin;

      // Calculate a plane-aligned bounding box.
      // Calculating the bounding box from the runtime object, instead
      // of a copy of the geometry, will produce a more accurate result.
      Transform world_to_plane = Transform.ChangeBasis(Plane.WorldXY, plane);
      BoundingBox bbox = obj.Geometry.GetBoundingBox(world_to_plane);

      // Move the cylinder's plane to the base of the bounding box.
      // Create a plane through the base of the bounding box.
      Plane bbox_plane = new Plane(
        bbox.Corner(true, true, true),
        bbox.Corner(false, true, true),
        bbox.Corner(true, false, true)
        );
      // Transform the plane to the world xy-plane
      Transform plane_to_world = Transform.ChangeBasis(plane, Plane.WorldXY);
      bbox_plane.Transform(plane_to_world);
      // Project the cylinder plane's origin onto the bounding box plane
      plane.Origin = bbox_plane.ClosestPoint(origin);

      // Cylinder height is bounding box height
      Point3d pt0 = bbox.Corner(true, true, true);
      Point3d pt1 = bbox.Corner(true, true, false);
      double height = pt0.DistanceTo(pt1);

      // Create a new cylinder
      Circle new_circle = new Circle(plane, circle.Radius);
      Cylinder new_cylinder = new Cylinder(new_circle, height);
      RevSurface rev_surface = new_cylinder.ToRevSurface();
      doc.Objects.AddSurface(rev_surface);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
