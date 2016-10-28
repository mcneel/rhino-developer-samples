using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace examples_cs
{
  public class EdgeSrfCommand : Command
  {
    public override string EnglishName { get { return "csEdgeSrf"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select 2, 3, or 4 open curves");
      go.GeometryFilter = ObjectType.Curve;
      go.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve;
      go.GetMultiple(2, 4);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var curves = go.Objects().Select(o => o.Curve());

      var brep = Brep.CreateEdgeSurface(curves);

      if (brep != null)
      {
        doc.Objects.AddBrep(brep);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}