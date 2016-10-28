import Rhino
import scriptcontext
import System.Guid

def AddSphere():
    center = Rhino.Geometry.Point3d(0, 0, 0)
    radius = 5.0
    sphere = Rhino.Geometry.Sphere(center, radius)
    if scriptcontext.doc.Objects.AddSphere(sphere)!=System.Guid.Empty:
        scriptcontext.doc.Views.Redraw()
        return Rhino.Commands.Result.Success
    return Rhino.Commands.Result.Failure


if __name__ == "__main__":
    AddSphere()
