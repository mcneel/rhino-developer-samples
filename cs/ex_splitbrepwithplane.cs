using Rhino;
using Rhino.DocObjects;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Geometry;

namespace examples_cs
{
  public class SplitBrepsWithPlaneCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csSplitBrepsWithPlane"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      //First, collect all the breps to split
      ObjRef[] obj_refs;
      var rc = RhinoGet.GetMultipleObjects("Select breps to split", false, ObjectType.Brep, out obj_refs);
      if (rc != Result.Success || obj_refs == null)
        return rc;

      // Get the final plane
      Plane plane;
      rc = RhinoGet.GetPlane(out plane);
      if (rc != Result.Success)
        return rc;

      //Iterate over all object references
      foreach (var obj_ref in obj_refs)
      {
        var brep = obj_ref.Brep();
        var bbox = brep.GetBoundingBox(false);

        //Grow the boundingbox in all directions
        //If the boundingbox is flat (zero volume or even zero area) 
        //then the CreateThroughBox method will fail.
        var min_point = bbox.Min;
        min_point.X -= 1.0;
        min_point.Y -= 1.0;
        min_point.Z -= 1.0;
        bbox.Min = min_point;
        var max_point = bbox.Max;
        max_point.X += 1.0;
        max_point.Y += 1.0;
        max_point.Z += 1.0;
        bbox.Max = max_point;

        var plane_surface = PlaneSurface.CreateThroughBox(plane, bbox);
        if (plane_surface == null)
        {
          //This is rare, it will most likely not happen unless either the plane or the boundingbox are invalid
          RhinoApp.WriteLine("Cutting plane could not be constructed.");
        }
        else
        {
          var breps = brep.Split(plane_surface.ToBrep(), doc.ModelAbsoluteTolerance);
          if (breps == null || breps.Length == 0)
          {
            RhinoApp.Write("Plane does not intersect brep (id:{0})", obj_ref.ObjectId);
            continue;
          }
          foreach (var brep_piece in breps)
          {
            doc.Objects.AddBrep(brep_piece);
          }
          doc.Objects.AddSurface(plane_surface);
          doc.Objects.Delete(obj_ref, false);
        }
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}