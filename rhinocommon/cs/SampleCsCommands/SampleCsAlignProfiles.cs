using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsAlignProfiles : Command
  {
    public override string EnglishName => "SampleCsAlignProfiles";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      CurveObject[] curve_objs = new CurveObject[2];
      BoundingBox[] boxes = new BoundingBox[2];
      Rectangle3d[] rects = new Rectangle3d[2];

      for (int i = 0; i < 2; i++)
      {
        GetObject go = new GetObject { GeometryFilter = ObjectType.Curve, SubObjectSelect = false };
        if (i == 0)
        {
          go.SetCommandPrompt("Select curve to align to");
        }
        else
        {
          go.SetCommandPrompt("Select curve to change");
          go.EnablePreSelect(false, true);
          go.DeselectAllBeforePostSelect = false;
        }
        go.Get();
        if (go.CommandResult() != Result.Success)
          return go.CommandResult();

        if (!(go.Object(0).Object() is CurveObject curve_obj))
          return Result.Failure;

        bool rc = RhinoObject.GetTightBoundingBox(new[] { curve_obj }, out BoundingBox box);
        if (!rc || !box.IsValid)
        {
          RhinoApp.WriteLine("Cannot calculate bounding box.");
          return Result.Failure;
        }

        rc = IsBoundingBoxRectangle(box, doc.ModelAbsoluteTolerance, out Rectangle3d rect);
        if (!rc)
        {
          RhinoApp.WriteLine("Profile is not planar in world coordinates.");
          return Result.Failure;
        }

        curve_objs[i] = curve_obj;
        boxes[i] = box;
        rects[i] = rect;
      }

      Vector3d shared_axis;
      double source_width, target_width;

      if (rects[0].Plane.XAxis == rects[1].Plane.XAxis)
      {
        shared_axis = rects[0].Plane.XAxis;
        source_width = rects[0].Width;
        target_width = rects[1].Width;
      }
      else if (rects[0].Plane.XAxis == rects[1].Plane.YAxis)
      {
        shared_axis = rects[0].Plane.XAxis;
        source_width = rects[0].Width;
        target_width = rects[1].Height;
      }
      else if (rects[0].Plane.YAxis == rects[1].Plane.XAxis)
      {
        shared_axis = rects[0].Plane.YAxis;
        source_width = rects[0].Height;
        target_width = rects[1].Width;
      }
      else if (rects[0].Plane.YAxis == rects[1].Plane.YAxis)
      {
        shared_axis = rects[0].Plane.YAxis;
        source_width = rects[0].Height;
        target_width = rects[1].Height;
      }
      else
      {
        RhinoApp.WriteLine("Unable to align profiles - could not find a shared axis.");
        return Result.Failure;
      }

      Plane plane = new Plane(rects[0].Plane.Origin, shared_axis);
      double dist = plane.DistanceTo(rects[1].Plane.Origin);
      Vector3d trans = new Vector3d(shared_axis);
      trans *= -dist;

      Transform A = Transform.Translation(trans);

      double scale_factor = 1.0;
      if (target_width != 0)
        scale_factor = source_width / target_width;

      Transform B = Transform.Scale(rects[1].Plane.Origin, scale_factor);

      Transform xform = A * B;

      doc.Objects.Transform(curve_objs[1].Id, xform, true);
      doc.Objects.UnselectAll();
      doc.Views.Redraw();

      return Result.Success;
    }

    private bool IsBoundingBoxRectangle(BoundingBox box, double tolerance, out Rectangle3d rect)
    {
      rect = Rectangle3d.Unset;

      if (box.IsDegenerate(tolerance) != 1)
        return false;

      Point3d origin = box.Min;

      if (IsBoundingBoxDegenerateX(box, tolerance))
      {
        Plane plane = new Plane(origin, Vector3d.YAxis, Vector3d.ZAxis);
        double width = box.Max.Y - box.Min.Y;
        double height = box.Max.Z - box.Min.Z;
        rect = new Rectangle3d(plane, width, height);
      }
      else if (IsBoundingBoxDegenerateY(box, tolerance))
      {
        Plane plane = new Plane(origin, Vector3d.XAxis, Vector3d.ZAxis);
        double width = box.Max.X - box.Min.X;
        double height = box.Max.Z - box.Min.Z;
        rect = new Rectangle3d(plane, width, height);
      }
      else if (IsBoundingBoxDegenerateZ(box, tolerance))
      {
        Plane plane = new Plane(origin, Vector3d.XAxis, Vector3d.YAxis);
        double width = box.Max.X - box.Min.X;
        double height = box.Max.Y - box.Min.Y;
        rect = new Rectangle3d(plane, width, height);
      }
      return rect.IsValid;
    }

    private bool IsBoundingBoxDegenerateX(BoundingBox box, double tolerance)
    {
      return box.Max.X - box.Min.X < tolerance;
    }

    private bool IsBoundingBoxDegenerateY(BoundingBox box, double tolerance)
    {
      return box.Max.Y - box.Min.Y < tolerance;
    }

    private bool IsBoundingBoxDegenerateZ(BoundingBox box, double tolerance)
    {
      return box.Max.Z - box.Min.Z < tolerance;
    }
  }
}