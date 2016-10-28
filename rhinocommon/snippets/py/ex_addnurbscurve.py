import Rhino
import scriptcontext
import System.Guid

def AddNurbsCurve():
    points = Rhino.Collections.Point3dList(5)
    points.Add(0, 0, 0)
    points.Add(0, 2, 0)
    points.Add(2, 3, 0)
    points.Add(4, 2, 0)
    points.Add(4, 0, 0)

    nc = Rhino.Geometry.NurbsCurve.Create(False, 3, points)
    rc = Rhino.Commands.Result.Failure
    if nc and nc.IsValid:
        if scriptcontext.doc.Objects.AddCurve(nc)!=System.Guid.Empty:
            scriptcontext.doc.Views.Redraw()
            rc = Rhino.Commands.Result.Success
    return rc

if __name__=="__main__":
    AddNurbsCurve()
