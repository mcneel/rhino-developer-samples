import Rhino
import scriptcontext
import System.Guid

def AddTorus():
    major_radius = 4.0
    minor_radius = 2.0

    plane = Rhino.Geometry.Plane.WorldXY
    torus = Rhino.Geometry.Torus(plane, major_radius, minor_radius)
    revsrf = torus.ToRevSurface()

    if scriptcontext.doc.Objects.AddSurface(revsrf)!=System.Guid.Empty:
        scriptcontext.doc.Views.Redraw()
        return Rhino.Commands.Result.Success
    return Rhino.Commands.Result.Failure


if __name__=="__main__":
    AddTorus()
