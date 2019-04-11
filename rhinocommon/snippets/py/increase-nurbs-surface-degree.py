from Rhino import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.DocObjects import *
from scriptcontext import doc

def RunCommand():
    rc, obj_ref = RhinoGet.GetOneObject("Select surface", False, ObjectType.Surface)
    if rc != Result.Success: return rc
    if obj_ref == None: return Result.Failure
    surface = obj_ref.Surface()
    if surface == None: return Result.Failure
    nurbs_surface = surface.ToNurbsSurface()

    new_u_degree = -1
    rc, new_u_degree = RhinoGet.GetInteger("New U degree <{0}...11>".format(nurbs_surface.Degree(0)), True, new_u_degree, nurbs_surface.Degree(0), 11)
    if rc != Result.Success: return rc

    new_v_degree = -1
    rc, new_v_degree = RhinoGet.GetInteger("New V degree <{0}...11>".format(nurbs_surface.Degree(1)), True, new_v_degree, nurbs_surface.Degree(1), 11)
    if rc != Result.Success: return rc

    rc = Result.Failure
    if nurbs_surface.IncreaseDegreeU(new_u_degree):
        if nurbs_surface.IncreaseDegreeV(new_v_degree):
            if doc.Objects.Replace(obj_ref.ObjectId, nurbs_surface):
                rc = Result.Success

    print "Result: {0}".format(rc)
    doc.Views.Redraw()
    return rc

if __name__=="__main__":
    RunCommand()
