from Rhino import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.Input.Custom import *

def RunCommand():
  gp = GetPoint()
  gp.SetCommandPrompt("Base point")
  gp.Get()
  if gp.CommandResult() <> Result.Success:
    return gp.CommandResult()
  base_point = gp.Point()

  gp.SetCommandPrompt("First reference point")
  gp.DrawLineFromPoint(base_point, True)
  gp.Get()
  if gp.CommandResult() <> Result.Success:
    return gp.CommandResult()
  first_point = gp.Point()


  rc, angle_radians = RhinoGet.GetAngle("Second reference point", base_point, first_point, 0)
  if rc == Result.Success:
    print "Angle = {0} degrees".format(RhinoMath.ToDegrees(angle_radians))
  return rc

if __name__ == "__main__":
  RunCommand()
