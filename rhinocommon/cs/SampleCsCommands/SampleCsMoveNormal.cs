using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsMoveNormal : Command
  {
    public override string EnglishName => "SampleCsMoveNormal";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var rc = RhinoGet.GetMultipleObjects("Select objects to move", false, ObjectType.AnyObject, out var objrefs);
      if (rc != Result.Success)
        return rc;

      var go = new GetObject();
      go.SetCommandPrompt("Select surface, polysurface, or SubD for normal direction");
      go.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go.EnablePreSelect(false, true);
      go.DeselectAllBeforePostSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var rh_obj = go.Object(0).Object();
      var brep = go.Object(0).Brep();
      if (null == rh_obj || null == brep)
        return Result.Failure;

      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to move normal from");
      gp.Constrain(brep, rh_obj.Attributes.WireDensity, -1, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var res = brep.ClosestPoint(
        gp.Point(), 
        out var base_pt, 
        out var ci, 
        out var s, 
        out var t, 
        doc.ModelAbsoluteTolerance, 
        out var normal
        );

      if (!res || ci.ComponentIndexType != ComponentIndexType.BrepFace)
        return Result.Failure;

      gp.SetCommandPrompt("Point to move normal to");
      gp.ClearConstraints();
      gp.SetBasePoint(base_pt, true);
      gp.DrawLineFromPoint(base_pt, true);
      gp.Constrain(base_pt, base_pt + normal);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var dir = gp.Point() - base_pt;
      if (!dir.IsTiny())
      {
        var xform = Transform.Translation(dir);

        foreach (var objref in objrefs)
        {
          var obj = objref.Object();
          if (null != obj)
            doc.Objects.Transform(objref, xform, true);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }

}