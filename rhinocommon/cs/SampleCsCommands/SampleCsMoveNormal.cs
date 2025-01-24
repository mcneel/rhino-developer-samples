using Rhino;
using Rhino.Commands;
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
      Result rc = RhinoGet.GetMultipleObjects("Select objects to move", false, ObjectType.AnyObject, out ObjRef[] objrefs);
      if (rc != Result.Success)
        return rc;

      GetObject go = new GetObject();
      go.SetCommandPrompt("Select surface, polysurface, or SubD for normal direction");
      go.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go.EnablePreSelect(false, true);
      go.DeselectAllBeforePostSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      RhinoObject rh_obj = go.Object(0).Object();
      Brep brep = go.Object(0).Brep();
      if (null == rh_obj || null == brep)
        return Result.Failure;

      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Point to move normal from");
      gp.Constrain(brep, rh_obj.Attributes.WireDensity, -1, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      bool res = brep.ClosestPoint(
        gp.Point(),
        out Point3d base_pt,
        out ComponentIndex ci,
        out double s,
        out double t,
        doc.ModelAbsoluteTolerance,
        out Vector3d normal
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

      Vector3d dir = gp.Point() - base_pt;
      if (!dir.IsTiny())
      {
        Transform xform = Transform.Translation(dir);

        foreach (ObjRef objref in objrefs)
        {
          RhinoObject obj = objref.Object();
          if (null != obj)
            doc.Objects.Transform(objref, xform, true);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }

}