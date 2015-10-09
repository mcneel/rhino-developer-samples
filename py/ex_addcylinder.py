import Rhino
import scriptcontext
import System.Guid

def AddCylinder():
    center_point = Rhino.Geometry.Point3d(0, 0, 0)
    height_point = Rhino.Geometry.Point3d(0, 0, 10)
    zaxis = height_point-center_point
    plane = Rhino.Geometry.Plane(center_point, zaxis)
    radius = 5
    circle = Rhino.Geometry.Circle(plane, radius)
    cylinder = Rhino.Geometry.Cylinder(circle, zaxis.Length)
    brep = cylinder.ToBrep(True, True)
    if brep:
        if scriptcontext.doc.Objects.AddBrep(brep)!=System.Guid.Empty:
            scriptcontext.doc.Views.Redraw()
            return Rhino.Commands.Result.Success
    return Rhino.Commands.Result.Failure

if __name__=="__main__":
    AddCylinder()
