using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsDeleteMeshFace : Command
  {
    public override string EnglishName => "SampleCsDeleteMeshFace";

    private struct FaceInfo
    {
      public MeshObject MeshObject;
      public int FaceIndex;
    }

    private static int CompareFaceInfo(FaceInfo a, FaceInfo b)
    {
      if (a.MeshObject.RuntimeSerialNumber < b.MeshObject.RuntimeSerialNumber)
        return -1;
      if (a.MeshObject.RuntimeSerialNumber > b.MeshObject.RuntimeSerialNumber)
        return 1;
      return 0;
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select mesh faces to delete");
      go.GeometryFilter = ObjectType.MeshFace;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      List<FaceInfo> face_list = new List<FaceInfo>(go.ObjectCount);
      for (int i = 0; i < go.ObjectCount; i++)
      {
        ObjRef obj_ref = go.Object(i);
        MeshObject obj = obj_ref.Object() as Rhino.DocObjects.MeshObject;
        if (null == obj)
          return Result.Failure;

        FaceInfo fi = new FaceInfo();
        fi.MeshObject = obj;
        fi.FaceIndex = obj_ref.GeometryComponentIndex.Index;
        face_list.Add(fi);
      }

      face_list.Sort(CompareFaceInfo);

      List<int> face_indices = new List<int>();
      int idx = 1;

      Mesh new_mesh = null;

      MeshObject curr_obj = face_list[0].MeshObject;
      face_indices.Add(face_list[0].FaceIndex);
      while (idx < face_list.Count)
      {
        var next_obj = face_list[idx].MeshObject;
        if (curr_obj.RuntimeSerialNumber == next_obj.RuntimeSerialNumber)
        {
          face_indices.Add(face_list[idx].FaceIndex);
          curr_obj = next_obj;
          idx++;
          continue;
        }

        new_mesh = curr_obj.MeshGeometry.DuplicateMesh();
        face_indices.Sort();
        new_mesh.Faces.DeleteFaces(face_indices);
        new_mesh.Compact();
        doc.Objects.Replace(curr_obj.Id, new_mesh);

        face_indices.Clear();
        face_indices.Add(face_list[idx].FaceIndex);
        curr_obj = next_obj;
        idx++;
      }

      new_mesh = curr_obj.MeshGeometry.DuplicateMesh();
      face_indices.Sort();
      new_mesh.Faces.DeleteFaces(face_indices);
      new_mesh.Compact();
      doc.Objects.Replace(curr_obj.Id, new_mesh);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
