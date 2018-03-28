using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsBooleanDifference2 : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsBooleanDifference2"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go0 = new GetObject();
      go0.SetCommandPrompt("Select surface or polysurface to subtract from");
      go0.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go0.SubObjectSelect = false;
      go0.Get();
      if (go0.CommandResult() != Result.Success)
        return go0.CommandResult();

      var brep0 = go0.Object(0).Brep();
      if (null == brep0)
        return Result.Failure;

      var go1 = new GetObject();
      go1.SetCommandPrompt("Select surface or polysurface to subtract with");
      go1.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go1.SubObjectSelect = false;
      go1.EnablePreSelect(false, true);
      go1.DeselectAllBeforePostSelect = false;
      go1.Get();
      if (go1.CommandResult() != Result.Success)
        return go1.CommandResult();

      var brep1 = go1.Object(0).Brep();
      if (null == brep1)
        return Result.Failure;

      var tolerance = doc.ModelAbsoluteTolerance;
      var out_breps = Brep.CreateBooleanDifference(brep0, brep1, tolerance);
      if (null != out_breps && out_breps.Length > 0)
      {
        foreach (var b in out_breps)
          doc.Objects.AddBrep(b);

        doc.Objects.Delete(go0.Object(0).ObjectId, true);
        doc.Objects.Delete(go1.Object(0).ObjectId, true);
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}
