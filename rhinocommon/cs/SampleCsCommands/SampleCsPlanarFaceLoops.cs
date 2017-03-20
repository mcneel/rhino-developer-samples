using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("d8f632e9-8236-49a4-90e1-d7c8fe5e3ac1")]
  public class SampleCsPlanarFaceLoops : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsPlanarFaceLoops"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select planar surface
      var gs = new GetObject();
      gs.SetCommandPrompt("Select planar surface");
      gs.GeometryFilter = ObjectType.Surface;
      gs.SubObjectSelect = false;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      var brep_ref = gs.Object(0);
      var brep = brep_ref.Brep();
      if (null == brep)
        return Result.Failure;

      // Verify underlying surface is a PlaneSurface
      var plane_surface = brep.Faces[0].UnderlyingSurface() as PlaneSurface;
      if (null == plane_surface)
        return Result.Nothing;

      // Select trimming curves on planar surface
      var gc = new GetObject();
      gc.SetCommandPrompt("Select trimming curves on planar surface");
      gc.GeometryFilter = ObjectType.Curve;
      gc.GroupSelect = true;
      gc.SubObjectSelect = false;
      gc.EnablePreSelect(false, true);
      gc.DeselectAllBeforePostSelect = false;
      gc.GetMultiple(1, 0);
      if (gc.CommandResult() != Result.Success)
        return gc.CommandResult();

      // Make a copy of the selected Brep
      var new_brep = new Brep();
      new_brep.Append(brep);

      // Add each selected curve a a inner planar face loop
      var boundary = new Curve[1];
      for (var i = 0; i < gc.ObjectCount; i++)
      {
        var curve = gc.Object(i).Curve();
        if (null != curve)
        {
          boundary[0] = curve;
          new_brep.Loops.AddPlanarFaceLoop(0, BrepLoopType.Inner, boundary);
        }
      }

      new_brep.Compact();

      // IF all is well, then replace the selecte Brep with our new one
      if (new_brep.IsValid)
      {
        doc.Objects.Replace(brep_ref.ObjectId, new_brep);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
