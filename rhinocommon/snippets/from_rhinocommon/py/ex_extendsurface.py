import Rhino
from Rhino.Input.Custom import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Geometry import *
from scriptcontext import doc

def RunCommand():
  go = Rhino.Input.Custom.GetObject()
  go.SetCommandPrompt("Select edge of surface to extend")
  go.GeometryFilter = ObjectType.EdgeFilter
  go.GeometryAttributeFilter = GeometryAttributeFilter.EdgeCurve
  go.Get()
  if go.CommandResult() != Result.Success:
    return go.CommandResult()
  obj_ref = go.Object(0)

  surface = obj_ref.Surface()
  if surface == None:
    print "Unable to extend polysurfaces."
    return Result.Failure

  brep = obj_ref.Brep()
  face = obj_ref.Face()
  if brep == None or face == None:
    return Result.Failure
  if face.FaceIndex < 0:
    return Result.Failure

  if not brep.IsSurface:
    print "Unable to extend trimmed surfaces."
    return Result.Nothing

  curve = obj_ref.Curve()

  trim = obj_ref.Trim()
  if trim == None:
    return Result.Failure

  if trim.TrimType == BrepTrimType.Seam:
    print "Unable to extend surface at seam."
    return Result.Nothing

  extended_surface = surface.Extend(trim.IsoStatus, 5.0, True)
  if extended_surface != None:
    mybrep = Brep.CreateFromSurface(extended_surface)
    doc.Objects.Replace(obj_ref.ObjectId, mybrep)
    doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()