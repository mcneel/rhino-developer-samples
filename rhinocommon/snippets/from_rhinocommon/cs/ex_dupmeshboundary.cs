using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.DocObjects;

namespace examples_cs
{
  public class DupMeshBoundaryCommand : Command
  {
    public override string EnglishName { get { return "csDupMeshBoundary"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gm = new GetObject();
      gm.SetCommandPrompt("Select open mesh");
      gm.GeometryFilter = ObjectType.Mesh;
      gm.GeometryAttributeFilter = GeometryAttributeFilter.OpenMesh;
      gm.Get();
      if (gm.CommandResult() != Result.Success)
        return gm.CommandResult();
      var mesh = gm.Object(0).Mesh();
      if (mesh == null)
        return Result.Failure;

      var polylines = mesh.GetNakedEdges();
      foreach (var polyline in polylines)
      {
        doc.Objects.AddPolyline(polyline);
      }

      return Result.Success;
    }
  }
}