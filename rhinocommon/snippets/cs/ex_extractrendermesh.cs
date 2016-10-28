using System;

partial class Examples
{
  public static Rhino.Commands.Result ExtractRenderMesh(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef objRef = null;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface", false, Rhino.DocObjects.ObjectType.Brep, out objRef);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    Rhino.DocObjects.RhinoObject obj = objRef.Object();
    if (null == obj)
      return Rhino.Commands.Result.Failure;

    System.Collections.Generic.List<Rhino.DocObjects.RhinoObject> objList = new System.Collections.Generic.List<Rhino.DocObjects.RhinoObject>(1);
    objList.Add(obj);

    Rhino.DocObjects.ObjRef[] meshObjRefs = Rhino.DocObjects.RhinoObject.GetRenderMeshes(objList, true, false);
    if (null != meshObjRefs)
    {
      for (int i = 0; i < meshObjRefs.Length; i++)
      {
        Rhino.DocObjects.ObjRef meshObjRef = meshObjRefs[i];
        if (null != meshObjRef)
        {
          Rhino.Geometry.Mesh mesh = meshObjRef.Mesh();
          if (null != mesh)
            doc.Objects.AddMesh(mesh);
        }
      }
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }
}
