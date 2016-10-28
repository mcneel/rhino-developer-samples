from Rhino.Commands import *
from Rhino.Input.Custom import *
from Rhino.DocObjects import *
from scriptcontext import doc

def RunCommand():
  gm = GetObject()
  gm.SetCommandPrompt("Select open mesh")
  gm.GeometryFilter = ObjectType.Mesh
  gm.GeometryAttributeFilter = GeometryAttributeFilter.OpenMesh
  gm.Get()
  if gm.CommandResult() != Result.Success:
    return gm.CommandResult()
  mesh = gm.Object(0).Mesh()
  if mesh == None:
    return Result.Failure

  polylines = mesh.GetNakedEdges()
  for polyline in polylines:
    doc.Objects.AddPolyline(polyline)
  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
    RunCommand()