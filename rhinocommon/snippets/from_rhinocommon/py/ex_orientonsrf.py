import Rhino
import scriptcontext
import System.Guid

def OrientOnSrf():
    # Select objects to orient
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select objects to orient")
    go.SubObjectSelect = False
    go.GroupSelect = True
    go.GetMultiple(1, 0)
    if go.CommandResult()!=Rhino.Commands.Result.Success:
        return go.CommandResult()

    # Point to orient from
    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Point to orient from")
    gp.Get()
    if gp.CommandResult()!=Rhino.Commands.Result.Success:
        return gp.CommandResult()
    
    # Define source plane
    view = gp.View()
    if not view:
        view = doc.Views.ActiveView
        if not view: return Rhino.Commands.Result.Failure

    source_plane = view.ActiveViewport.ConstructionPlane()
    source_plane.Origin = gp.Point()

    # Surface to orient on
    gs = Rhino.Input.Custom.GetObject()
    gs.SetCommandPrompt("Surface to orient on")
    gs.GeometryFilter = Rhino.DocObjects.ObjectType.Surface
    gs.SubObjectSelect = True
    gs.DeselectAllBeforePostSelect = False
    gs.OneByOnePostSelect = True
    gs.Get()
    if gs.CommandResult()!=Rhino.Commands.Result.Success:
        return gs.CommandResult()

    objref = gs.Object(0)
    # get selected surface object
    obj = objref.Object()
    if not obj: return Rhino.Commands.Result.Failure
    # get selected surface (face)
    surface = objref.Surface()
    if not surface: return Rhino.Commands.Result.Failure
    # Unselect surface
    obj.Select(False)

    # Point on surface to orient to
    gp.SetCommandPrompt("Point on surface to orient to")
    gp.Constrain(surface, False)
    gp.Get()
    if gp.CommandResult()!=Rhino.Commands.Result.Success:
        return gp.CommandResult()

    # Do transformation
    rc = Rhino.Commands.Result.Failure
    getrc, u, v = surface.ClosestPoint(gp.Point())
    if getrc:
        getrc, target_plane = surface.FrameAt(u,v)
        if getrc:
            # Build transformation
            xform = Rhino.Geometry.Transform.PlaneToPlane(source_plane, target_plane)
            # Do the transformation. In this example, we will copy the original objects
            delete_original = False
            for i in range(go.ObjectCount):
                rhobj = go.Object(i)
                scriptcontext.doc.Objects.Transform(rhobj, xform, delete_original)
            scriptcontext.doc.Views.Redraw()
            rc = Rhino.Commands.Result.Success
    return rc


if __name__=="__main__":
    OrientOnSrf()
