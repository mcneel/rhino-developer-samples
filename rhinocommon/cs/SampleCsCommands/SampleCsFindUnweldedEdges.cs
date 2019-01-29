using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsFindUnweldedEdges : Command
  {
    public override string EnglishName => "SampleCsFindUnweldedEdges";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select mesh");
      go.GeometryFilter = ObjectType.Mesh;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var mesh = go.Object(0).Mesh();
      if (null == mesh)
        return Result.Failure;

      var unwelded_count = 0;
      for (var topei = 0; topei < mesh.TopologyEdges.Count; topei++)
      {
        if (IsUweldedMeshEdge(mesh, topei))
        {
          RhinoApp.WriteLine("Edge({0}) is unwelded", topei);
          unwelded_count++;
        }
      }
      RhinoApp.WriteLine("Unwelded edge count = {0}", unwelded_count);

      return Result.Success;
    }

    /// <summary>
    /// Returns true if a topology mesh edge is unwelded.
    /// </summary>
    /// <param name="mesh">The mesh.</param>
    /// <param name="topei">The topology mesh edge index.</param>
    /// <returns>True if the edge is uwelded, false otherwise.</returns>
    /// <remarks>
    /// Topology mesh edges that have unique vertices along that edge are considered unwelded.
    /// Likewise, topology mesh edges that share vertices along that edge are considered welded.
    /// </remarks>
    public static bool IsUweldedMeshEdge(Mesh mesh, int topei)
    {
      if (null == mesh)
        throw new ArgumentNullException(nameof(mesh));

      if (topei < 0 || topei >= mesh.TopologyEdges.Count)
        throw new IndexOutOfRangeException(nameof(topei));

      var topfi = mesh.TopologyEdges.GetConnectedFaces(topei);
      if (1 == topfi.Length)
        return true;

      var topvi = mesh.TopologyEdges.GetTopologyVertices(topei);
      var topvi0 = mesh.TopologyVertices.MeshVertexIndices(topvi.I);
      var topvi1 = mesh.TopologyVertices.MeshVertexIndices(topvi.J);

      // Both ends of the edge must have more than 1 mesh vertex or they are welded.
      // However, having more than 1 vertex at both ends does not necessarily mean it is unwelded.
      if (1 == topvi0.Length || 1 == topvi1.Length)
        return false;

      var pta = mesh.Vertices[topvi0[0]];
      var ptb = mesh.Vertices[topvi1[0]];
      var pta_indexes = new List<int>(topvi0.Length);
      var ptb_indexes = new List<int>(topvi1.Length);

      var ict = topfi.Length;
      for (var i = 0; ict > i; i++)
      {
        var face = mesh.Faces[topfi[i]];
        var jct = face.IsQuad ? 4 : 3;
        for (var j = 0; j < jct; j++)
        {
          if (pta == mesh.Vertices[face[j]])
          {
            if (0 == pta_indexes.Count)
            {
              pta_indexes.Add(face[j]);
            }
            else
            {
              var kct = pta_indexes.Count;
              for (var k = 0; k < kct; k++)
              {
                if (pta_indexes[k] == face[j])
                  return false;
              }

              pta_indexes.Add(face[j]);
            }
          }
          else if (ptb == mesh.Vertices[face[j]])
          {
            if (0 == ptb_indexes.Count)
            {
              ptb_indexes.Add(face[j]);
            }
            else
            {
              var kct = ptb_indexes.Count;
              for (var k = 0; k < kct; k++)
              {
                if (ptb_indexes[k] == face[j])
                  return false;
              }

              ptb_indexes.Add(face[j]);
            }
          }
        }
      }

      return true;
    }
  }
}
