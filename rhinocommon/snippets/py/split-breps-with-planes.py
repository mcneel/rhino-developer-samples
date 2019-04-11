from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.Geometry import *
from scriptcontext import doc

def RunCommand():
    #First, collect all the breps to split
    rc, obj_refs = RhinoGet.GetMultipleObjects("Select breps to split", False, ObjectType.Brep)
    if rc != Result.Success or obj_refs == None:
        return rc

    # Get the final plane
    rc, plane = RhinoGet.GetPlane()
    if rc != Result.Success:
        return rc

    #Iterate over all object references
    for obj_ref in obj_refs:
        brep = obj_ref.Brep()
        bbox = brep.GetBoundingBox(False)

        #Grow the boundingbox in all directions
        #If the boundingbox is flat (zero volume or even zero area)
        #then the CreateThroughBox method will fail.
        min_point = bbox.Min
        min_point.X -= 1.0
        min_point.Y -= 1.0
        min_point.Z -= 1.0
        bbox.Min = min_point
        max_point = bbox.Max
        max_point.X += 1.0
        max_point.Y += 1.0
        max_point.Z += 1.0
        bbox.Max = max_point

        plane_surface = PlaneSurface.CreateThroughBox(plane, bbox)
        if plane_surface == None:
            #This is rare, it will most likely not happen unless either the plane or the boundingbox are invalid
            RhinoApp.WriteLine("Cutting plane could not be constructed.")
        else:
            breps = brep.Split(plane_surface.ToBrep(), doc.ModelAbsoluteTolerance)
            if breps == None or breps.Length == 0:
                RhinoApp.Write("Plane does not intersect brep (id:{0})", obj_ref.ObjectId)
                continue
            for brep_piece in breps:
                doc.Objects.AddBrep(brep_piece)
            doc.Objects.AddSurface(plane_surface)
            doc.Objects.Delete(obj_ref, False)

    doc.Views.Redraw()
    return Result.Success

if __name__ == "__main__":
    RunCommand()
