using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("982b610f-434d-4aa7-90a1-39f064834c6e")]
  public class SampleCsExtrudeMeshFace : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsExtrudeMeshFace"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select a mesh
      var go = new GetObject();
      go.SetCommandPrompt("Select mesh face to extrude");
      go.GeometryFilter = ObjectType.MeshFace;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // Get the selected object reference
      var objref = go.Object(0);
      var rh_obj = objref.Object();
      if (null == rh_obj)
        return Result.Failure;

      // Get the base mesh
      var mesh = objref.Mesh();
      if (null == mesh)
        return Result.Failure;

      // Get the selected component
      var ci = objref.GeometryComponentIndex;
      if (ComponentIndexType.MeshFace != ci.ComponentIndexType)
        return Result.Failure;

      // Copy the mesh geometry
      var mesh_copy = mesh.DuplicateMesh();

      // Make sure the mesh has normals
      if (mesh_copy.FaceNormals.Count != mesh_copy.Faces.Count)
        mesh_copy.FaceNormals.ComputeFaceNormals();
      if (mesh_copy.Normals.Count != mesh_copy.Vertices.Count)
        mesh_copy.Normals.ComputeNormals();

      // Get the mesh face
      var face = mesh_copy.Faces[ci.Index];

      // Get the mesh face vertices
      var base_vertices = new Point3d[4];
      for (var i = 0; i < 4; i++)
        base_vertices[i] = mesh_copy.Vertices[face[i]];

      // Get the face normal and scale it by 5.0
      var offset = mesh_copy.FaceNormals[ci.Index] * (float)5.0;

      // Calculate the offset vertices
      var offset_vertices = new Point3d[4];
      for (var i = 0; i < 4; i++)
        offset_vertices[i] = base_vertices[i] + offset;

      // Delete the mesh face
      var faces_indices = new int[] { ci.Index };
      mesh_copy.Faces.DeleteFaces(faces_indices);

      // Add the base mesh face vertices
      var base_indices = new int[4];
      for (var i = 0; i < 4; i++)
        base_indices[i] = mesh_copy.Vertices.Add(base_vertices[i]);

      // Add the offset mesh face vertices
      var offset_indices = new int[4];
      for (var i = 0; i < 4; i++)
        offset_indices[i] = mesh_copy.Vertices.Add(offset_vertices[i]);

      // Add the new mesh faces
      mesh_copy.Faces.AddFace(base_indices[3], base_indices[0], offset_indices[0], offset_indices[3]);
      mesh_copy.Faces.AddFace(base_indices[0], base_indices[1], offset_indices[1], offset_indices[0]);
      mesh_copy.Faces.AddFace(base_indices[1], base_indices[2], offset_indices[2], offset_indices[1]);
      mesh_copy.Faces.AddFace(base_indices[2], base_indices[3], offset_indices[3], offset_indices[2]);
      mesh_copy.Faces.AddFace(offset_indices[0], offset_indices[1], offset_indices[2], offset_indices[3]);

      // Clean up
      mesh_copy.FaceNormals.ComputeFaceNormals();
      mesh_copy.Normals.ComputeNormals();
      mesh_copy.TextureCoordinates.Clear();
      mesh_copy.Compact();

      string log;
      if (!mesh_copy.IsValidWithLog(out log))
      {
        RhinoApp.WriteLine(log);
        return Result.Failure;
      }

      doc.Objects.Replace(rh_obj.Id, mesh_copy);
      //doc.Objects.AddMesh(mesh_copy);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
