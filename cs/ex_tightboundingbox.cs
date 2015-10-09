using Rhino;
using Rhino.Commands;
using System.Linq;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.DocObjects;
using System.Collections.Generic;

namespace examples_cs
{
  public class TightBoundingBoxCommand : Command
  {
    public override string EnglishName { get { return "csTightBoundingBox"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef obj_ref;
      var rc = RhinoGet.GetOneObject(
        "Select surface to split", true, ObjectType.Surface, out obj_ref);
      if (rc != Result.Success)
        return rc;
      var surface = obj_ref.Surface();
      if (surface == null)
        return Result.Failure;

      obj_ref = null;
      rc = RhinoGet.GetOneObject(
        "Select cutting curve", true, ObjectType.Curve, out obj_ref);
      if (rc != Result.Success)
        return rc;
      var curve = obj_ref.Curve();
      if (curve == null)
        return Result.Failure;

      var brep_face = surface as BrepFace;
      if (brep_face == null)
        return Result.Failure;

      var split_brep = brep_face.Split(
        new List<Curve> {curve}, doc.ModelAbsoluteTolerance);
      if (split_brep == null)
      {
        RhinoApp.WriteLine("Unable to split surface.");
        return Result.Nothing;
      }

      var meshes = Mesh.CreateFromBrep(split_brep);

      foreach (var mesh in meshes)
      {
        var bbox = mesh.GetBoundingBox(true);
        switch (bbox.IsDegenerate(doc.ModelAbsoluteTolerance))
        {
          case 3:
          case 2:
            return Result.Failure;
          case 1:
            // rectangle
            // box with 8 corners flattened to rectangle with 4 corners
            var rectangle_corners = bbox.GetCorners().Distinct().ToList();
            // add 1st point as last to close the loop
            rectangle_corners.Add(rectangle_corners[0]);
            doc.Objects.AddPolyline(rectangle_corners);
            doc.Views.Redraw();
            break;
          case 0: 
            // box
            var brep_box = new Box(bbox).ToBrep();
            doc.Objects.AddBrep(brep_box);
            doc.Views.Redraw();
            break;
        }
      }

      return Result.Success;
    }
  }
}