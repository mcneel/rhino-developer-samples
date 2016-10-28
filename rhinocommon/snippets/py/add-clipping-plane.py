import Rhino
import scriptcontext
import System.Guid

def AddClippingPlane():
    # Define the corners of the clipping plane
    rc, corners = Rhino.Input.RhinoGet.GetRectangle()
    if rc!=Rhino.Commands.Result.Success: return rc

    # Get the active view
    view = scriptcontext.doc.Views.ActiveView
    if view is None: return Rhino.Commands.Result.Failure

    p0, p1, p2, p3 = corners
    # Create a plane from the corner points
    plane = Rhino.Geometry.Plane(p0, p1, p3)

    # Add a clipping plane object to the document
    id = scriptcontext.doc.Objects.AddClippingPlane(plane, p0.DistanceTo(p1), p0.DistanceTo(p3), view.ActiveViewportID)
    if id!=System.Guid.Empty:
        scriptcontext.doc.Views.Redraw()
        return Rhino.Commands.Result.Success
    return Rhino.Commands.Result.Failure

if __name__=="__main__":
    AddClippingPlane()
