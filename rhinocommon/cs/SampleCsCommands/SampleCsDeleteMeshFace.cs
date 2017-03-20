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
  [System.Runtime.InteropServices.Guid("00f1e446-d410-44f8-a947-b729d2323317")]
  public class SampleCsDeleteMeshFace : Command
  {
    public SampleCsDeleteMeshFace()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsDeleteMeshFace"; }
    }

    struct FaceInfo
    {
      public MeshObject m_obj;
      public int m_idx;
    }

    static int CompareFaceInfo(FaceInfo a, FaceInfo b)
    {
      if (a.m_obj.RuntimeSerialNumber < b.m_obj.RuntimeSerialNumber)
        return -1;
      if (a.m_obj.RuntimeSerialNumber > b.m_obj.RuntimeSerialNumber)
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
        fi.m_obj = obj;
        fi.m_idx = obj_ref.GeometryComponentIndex.Index;
        face_list.Add(fi);
      }

      face_list.Sort(CompareFaceInfo);

      List<int> face_indices = new List<int>();
      int idx = 1;

      Mesh new_mesh = null;
      MeshObject next_obj = null;

      MeshObject curr_obj = face_list[0].m_obj;
      face_indices.Add(face_list[0].m_idx);
      while (idx < face_list.Count)
      {
        next_obj = face_list[idx].m_obj;
        if (curr_obj.RuntimeSerialNumber == next_obj.RuntimeSerialNumber)
        {
          face_indices.Add(face_list[idx].m_idx);
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
        face_indices.Add(face_list[idx].m_idx);
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
