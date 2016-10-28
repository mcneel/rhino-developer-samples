import Rhino
import scriptcontext
import System.Guid

def AddLinearDimension():
    rc, dimension = Rhino.Input.RhinoGet.GetLinearDimension()
    if rc==Rhino.Commands.Result.Success:
        if scriptcontext.doc.Objects.AddLinearDimension(dimension)==System.Guid.Empty:
            rc = Rhino.Commands.Result.Failure
        else:
            scriptcontext.doc.Views.Redraw()
    return rc


if __name__=="__main__":
    AddLinearDimension()
