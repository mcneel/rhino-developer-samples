import Rhino
from Rhino.Geometry import *
from Rhino.Input import RhinoGet
from Rhino.Commands import Result
from Rhino.DocObjects import ObjectType
import rhinoscriptsyntax as rs
from scriptcontext import doc

def RunCommand():
  rc, objRef = RhinoGet.GetOneObject("Select surface or polysurface to mesh", True,
                                     ObjectType.Surface | ObjectType.PolysrfFilter)
  if rc != Result.Success:
    return rc
  brep = objRef.Brep()
  if None == brep:
    return Result.Failure

  jaggedAndFaster = MeshingParameters.Coarse
  smoothAndSlower = MeshingParameters.Smooth
  defaultMeshParams = MeshingParameters.Default
  minimal = MeshingParameters.Minimal

  meshes = Mesh.CreateFromBrep(brep, smoothAndSlower)
  if meshes == None or meshes.Length == 0:
    return Result.Failure

  brepMesh = Mesh()
  for mesh in meshes:
    brepMesh.Append(mesh)
  doc.Objects.AddMesh(brepMesh)
  doc.Views.Redraw()

if __name__ == "__main__":
  RunCommand()
