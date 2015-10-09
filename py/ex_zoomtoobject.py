import Rhino
import scriptcontext

def ZoomToObject():
    rc, rhobject = Rhino.Input.RhinoGet.GetOneObject("Select object to zoom", False, Rhino.DocObjects.ObjectType.None)
    if rc != Rhino.Commands.Result.Success: return

    obj = rhobject.Object()
    view = scriptcontext.doc.Views.ActiveView
    if not obj or not view: return

    bbox = obj.Geometry.GetBoundingBox(True)

    pad = 0.05  #A little padding...
    dx = (bbox.Max.X - bbox.Min.X) * pad
    dy = (bbox.Max.Y - bbox.Min.Y) * pad
    dz = (bbox.Max.Z - bbox.Min.Z) * pad
    bbox.Inflate(dx, dy, dz);
    view.ActiveViewport.ZoomBoundingBox(bbox)
    view.Redraw()

if __name__=="__main__":
    ZoomToObject()