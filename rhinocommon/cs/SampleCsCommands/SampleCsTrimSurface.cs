using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.FileIO;
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
      var gc = new GetObject();
      gc.SetCommandPrompt("Select cutting surface");
      gc.GeometryFilter = ObjectType.Surface;
      gc.SubObjectSelect = false;
      gc.Get();
      if (gc.CommandResult() != Result.Success)
        return gc.CommandResult();

      var splitter = gc.Object(0).Brep();
      if (null == splitter)
        return Result.Failure;

      // Select surface to trim
      var gs = new GetObject();
      gs.SetCommandPrompt("Select surface to trim");
      gs.GeometryFilter = ObjectType.Surface;
      gs.SubObjectSelect = false;
      gs.EnablePreSelect(false, true);
      gs.DeselectAllBeforePostSelect = false;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      var brep_ref = gs.Object(0);
      var brep = brep_ref.Brep();
      if (null == brep)
        return Result.Failure;

      var pick_pt = brep_ref.SelectionPoint();
      if (!pick_pt.IsValid)
      {
        // The user didn't "pick" the object, but rather
        // selected the object using the SelID command.
        // So, make up some pick location.
        var dom_u = brep.Faces[0].Domain(0);
        var dom_v = brep.Faces[0].Domain(1);
        pick_pt = brep.Faces[0].PointAt(dom_u.Min, dom_v.Min);
      }

      // Do the splitting
      var trims = brep.Split(splitter, doc.ModelAbsoluteTolerance);
      if (null == trims || 1 == trims.Length)
      {
        RhinoApp.WriteLine("Unable to trim surface.");
        return Result.Failure;
      }

      // Figure out which piece the user wanted trimmed away
      var dist = RhinoMath.UnsetValue;
      int picked_index = -1;
      for (int i = 0; i < trims.Length; i++)
      {
        var pt = trims[i].ClosestPoint(pick_pt);
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
