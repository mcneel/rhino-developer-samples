import Rhino
import scriptcontext

def AddCone():
    plane = Rhino.Geometry.Plane.WorldXY
    height = 10
    radius = 5
    cone = Rhino.Geometry.Cone(plane, height, radius)
    if cone.IsValid:
        cap_bottom = True
        cone_brep = cone.ToBrep(cap_bottom)
        if cone_brep:
            scriptcontext.doc.Objects.AddBrep(cone_brep)
            scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    AddCone()
