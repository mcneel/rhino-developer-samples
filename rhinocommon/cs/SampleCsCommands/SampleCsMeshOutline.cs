using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsMeshOutline : Command
  {
    public override string EnglishName => "SampleCsMeshOutline";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select surfaces, polysurfaces, or meshes");
      go.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter | ObjectType.Mesh;
      go.GroupSelect = true;
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      List<Mesh> InMeshes = new List<Mesh>(go.ObjectCount);
      List<RhinoObject> InMeshObjects = new List<RhinoObject>(go.ObjectCount);
      List<RhinoObject> InObjects = new List<RhinoObject>(go.ObjectCount);

      for (int i = 0; i < go.ObjectCount; i++)
      {
        ObjRef objRef = go.Object(i);
        Mesh mesh = objRef.Mesh();
        if (null == mesh)
          InObjects.Add(objRef.Object());
        else
        {
          InMeshes.Add(mesh);
          InMeshObjects.Add(objRef.Object());
        }
      }

      if (InObjects.Count > 0)
      {
        var meshRefs = RhinoObject.GetRenderMeshes(InObjects, true, false);
        if (null != meshRefs)
        {
          for (int i = 0; i < meshRefs.Length; i++)
          {
            Mesh mesh = meshRefs[i].Mesh();
            if (null != mesh)
              InMeshes.Add(mesh);
          }
        }
      }

      RhinoViewport vp = doc.Views.ActiveView.ActiveViewport;

      for (int i = 0; i < InMeshes.Count; i++)
      {
        Polyline[] plines = InMeshes[i].GetOutlines(vp);
        if (null != plines)
        {
          for (int j = 0; j < plines.Length; j++)
          {
            Rhino.Geometry.PolylineCurve plineCrv = new PolylineCurve(plines[j]);
            plineCrv.RemoveShortSegments(RhinoMath.SqrtEpsilon);
            if (plineCrv.IsValid)
            {
              Guid objId = doc.Objects.AddCurve(plineCrv);
              RhinoObject obj = doc.Objects.Find(objId);
              if (null != obj)
                obj.Select(true);
            }
          }
        }
      }

      for (int i = 0; i < InObjects.Count; i++)
        InObjects[i].Select(false);
      for (int i = 0; i < InMeshObjects.Count; i++)
        InMeshObjects[i].Select(false);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
