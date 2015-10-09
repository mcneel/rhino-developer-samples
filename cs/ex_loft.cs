using System.Linq;
using Rhino;
using Rhino.Input.Custom;
using Rhino.DocObjects;
using Rhino.Commands;
using Rhino.Geometry;

namespace examples_cs
{
  public class LoftCommand : Command
  {
    public override string EnglishName { get { return "csLoft"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // select curves to loft
      var gs = new GetObject();
      gs.SetCommandPrompt("select curves to loft");
      gs.GeometryFilter = ObjectType.Curve;
      gs.DisablePreSelect();
      gs.SubObjectSelect = false;
      gs.GetMultiple(2, 0);
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      var curves = gs.Objects().Select(obj => obj.Curve()).ToList();

      var breps = Brep.CreateFromLoft(curves, Point3d.Unset, Point3d.Unset, LoftType.Tight, false);
      foreach (var brep in breps)
        doc.Objects.AddBrep(brep);

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}