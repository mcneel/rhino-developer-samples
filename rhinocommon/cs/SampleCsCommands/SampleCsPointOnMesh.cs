using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("216a9b4b-9991-463e-af00-929b1994223e")]
  public class SampleCsPointOnMesh : Command
  {
    public SampleCsPointOnMesh()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsPointOnMesh"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Pick a mesh
      ObjRef obj_ref;
      Result rc = RhinoGet.GetOneObject("Select mesh", false, ObjectType.Mesh, out obj_ref);
      if (rc != Result.Success)
        return rc;

      Rhino.Geometry.Mesh mesh = obj_ref.Mesh();
      if (null == mesh)
        return Result.Failure;
  
      // Pick a point that is contrained to the mesh
      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Pick point on mesh");
      gp.Constrain(mesh, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      Point3d point = gp.Point();

      doc.Objects.AddPoint(point);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
