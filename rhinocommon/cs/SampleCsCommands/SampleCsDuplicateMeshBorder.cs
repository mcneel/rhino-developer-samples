using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace SampleCsCommands
{
  public class SampleCsDuplicateMeshBorder : Command
  {
    public SampleCsDuplicateMeshBorder()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsDuplicateMeshBorder"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.ObjRef objref;
      Result rc = Rhino.Input.RhinoGet.GetOneObject("Select mesh", false, ObjectType.Mesh, out objref);
      if (rc != Result.Success)
        return rc;
      if (null == objref)
        return Result.Failure;
      
      Rhino.Geometry.Mesh mesh = objref.Mesh();
      if (null == mesh)
        return Result.Failure;

      Rhino.Geometry.Collections.MeshTopologyEdgeList mesh_tope = mesh.TopologyEdges;
      for (int i = 0; i < mesh_tope.Count; i++)
      {
        // Find naked edges - edges with a single connecting face
        int[] mesh_topf = mesh_tope.GetConnectedFaces(i);
        if (null != mesh_topf && mesh_topf.Length == 1)
        {
          Guid id = doc.Objects.AddLine(mesh_tope.EdgeLine(i));
          doc.Objects.Select(id);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
