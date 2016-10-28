import Rhino
import scriptcontext
import rhinoscriptsyntax as rs

def RunCommand():
  rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select curve", True, Rhino.DocObjects.ObjectType.Curve)
  if(rc!= Rhino.Commands.Result.Success):
    return rc
  crv = objref.Curve()
  if( crv == None ):
    return Rhino.Commands.Result.Failure

  gp = Rhino.Input.Custom.GetPoint()
  gp.SetCommandPrompt("Pick a location on the curve")
  gp.Constrain(crv, False)
  gp.Get()
  if (gp.CommandResult() != Rhino.Commands.Result.Success):
    return gp.CommandResult();

  p = gp.Point()
  b, cp = crv.ClosestPoint(p)
  if (b):
    print "point: ({0},{1},{2}), parameter: {3}".format(p.X, p.Y, p.Z, cp)
    scriptcontext.doc.Objects.AddPoint(p)
    scriptcontext.doc.Views.Redraw()

  return Rhino.Commands.Result.Success

if __name__=="__main__":
  RunCommand()
