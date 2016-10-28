import Rhino
import scriptcontext

def UnrollSurface2():
    filter = Rhino.DocObjects.ObjectType.Brep | Rhino.DocObjects.ObjectType.Surface
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select surface or brep to unroll", False, filter)
    if rc!=Rhino.Commands.Result.Success: return rc;
    
    unroll = Rhino.Geometry.Unroller(objref.Geometry())
    mesh = objref.Brep().Faces[0].GetMesh()
    if not mesh: return Rhino.Commands.Result.Cancel

    unroll.AddFollowingGeometry(mesh.Vertices.ToPoint3dArray())
    unroll.ExplodeOutput = False
    breps, curves, points, dots = unroll.PerformUnroll()
    # change the mesh vertices to the flattened form and add it to the document
    if points.Length==mesh.Vertices.Count:
        for i, point in enumerate(points): mesh.Vertices.SetVertex(i, point)
        mesh.Normals.ComputeNormals()
    scriptcontext.doc.Objects.AddMesh(mesh, objref.Object().Attributes)
    scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    UnrollSurface2()
