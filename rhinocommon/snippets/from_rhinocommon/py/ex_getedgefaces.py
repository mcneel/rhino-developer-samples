from Rhino import *
from Rhino.Commands import *
from Rhino.DocObjects import *
from Rhino.Input import *
from scriptcontext import doc

def RunCommand():
  rc, obj_ref = RhinoGet.GetOneObject(
    "Select edge curve", False, ObjectType.EdgeFilter)
  if rc <> Result.Success:
    return rc
  edge = obj_ref.Edge()

  face_idxs = edge.AdjacentFaces()
  edge_owning_brep = edge.Brep

  for idx in face_idxs:
    face = edge_owning_brep.Faces[idx]
    face_copy = face.DuplicateFace(True)
    id = doc.Objects.AddBrep(face_copy)
    doc.Objects.Find(id).Select(True)

  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()