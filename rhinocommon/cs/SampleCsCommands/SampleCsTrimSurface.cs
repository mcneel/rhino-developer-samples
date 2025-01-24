using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsTrimSurface : Command
  {
    public override string EnglishName => "SampleCsTrimSurface";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select cutting surface
      GetObject gc = new GetObject();
      gc.SetCommandPrompt("Select cutting surface");
      gc.GeometryFilter = ObjectType.Surface;
      gc.SubObjectSelect = false;
      gc.Get();
      if (gc.CommandResult() != Result.Success)
        return gc.CommandResult();

      Brep splitter = gc.Object(0).Brep();
      if (null == splitter)
        return Result.Failure;

      // Select surface to trim
      GetObject gs = new GetObject();
      gs.SetCommandPrompt("Select surface to trim");
      gs.GeometryFilter = ObjectType.Surface;
      gs.SubObjectSelect = false;
      gs.EnablePreSelect(false, true);
      gs.DeselectAllBeforePostSelect = false;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      ObjRef brep_ref = gs.Object(0);
      Brep brep = brep_ref.Brep();
      if (null == brep)
        return Result.Failure;

      Point3d pick_pt = brep_ref.SelectionPoint();
      if (!pick_pt.IsValid)
      {
        // The user didn't "pick" the object, but rather
        // selected the object using the SelID command.
        // So, make up some pick location.
        Interval dom_u = brep.Faces[0].Domain(0);
        Interval dom_v = brep.Faces[0].Domain(1);
        pick_pt = brep.Faces[0].PointAt(dom_u.Min, dom_v.Min);
      }

      // Do the splitting
      Brep[] trims = brep.Split(splitter, doc.ModelAbsoluteTolerance);
      if (null == trims || 1 == trims.Length)
      {
        RhinoApp.WriteLine("Unable to trim surface.");
        return Result.Failure;
      }

      // Figure out which piece the user wanted trimmed away
      double dist = RhinoMath.UnsetValue;
      int picked_index = -1;
      for (int i = 0; i < trims.Length; i++)
      {
        Point3d pt = trims[i].ClosestPoint(pick_pt);
        if (pt.IsValid)
        {
          double d = pt.DistanceTo(pick_pt);
          if (!RhinoMath.IsValidDouble(dist) || d < dist)
          {
            dist = d;
            picked_index = i;
          }
        }
      }

      // Add the new pieces
      for (int i = 0; i < trims.Length; i++)
      {
        if (i != picked_index)
          doc.Objects.AddBrep(trims[i]);
      }

      // Delete the original
      doc.Objects.Delete(brep_ref, false);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
