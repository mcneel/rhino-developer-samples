import rhinoscriptsyntax as rs
from scriptcontext import *
import Rhino
from Rhino.Commands import Result

def RunCommand():
  # select a surface
  gs = Rhino.Input.Custom.GetObject()
  gs.SetCommandPrompt("select surface")
  gs.GeometryFilter = Rhino.DocObjects.ObjectType.Surface
  gs.DisablePreSelect()
  gs.SubObjectSelect = False
  gs.Get()
  if gs.CommandResult() != Result.Success:
    return gs.CommandResult()

  # get the selected face
  face = gs.Object(0).Face()
  if face == None:
    return

  # pick a point on the surface.  Constain
  # picking to the face.
  gp = Rhino.Input.Custom.GetPoint()
  gp.SetCommandPrompt("select point on surface")
  gp.Constrain(face, False)
  gp.Get()
  if gp.CommandResult() != Result.Success:
    return gp.CommandResult()

  # get the parameters of the point on the
  # surface that is clesest to gp.Point()
  b, u, v = face.ClosestPoint(gp.Point())
  if b:
    dir = face.NormalAt(u, v)
    if face.OrientationIsReversed:
      dir.Reverse()
    print "Surface normal at uv({0:f},{1:f}) = ({2:f},{3:f},{4:f})".format(
        u, v, dir.X, dir.Y, dir.Z)

if __name__ == "__main__":
  RunCommand()
