import Rhino
import scriptcontext
import System.Guid

def AddAnnotationText():
    pt = Rhino.Geometry.Point3d(10, 0, 0)
    text = "Hello RhinoCommon"
    height = 2.0
    font = "Arial"
    plane = scriptcontext.doc.Views.ActiveView.ActiveViewport.ConstructionPlane()
    plane.Origin = pt
    id = scriptcontext.doc.Objects.AddText(text, plane, height, font, False, False)
    if id!=System.Guid.Empty:
        scriptcontext.doc.Views.Redraw()
        return Rhino.Commands.Result.Success
    return Rhino.Commands.Result.Failure


if __name__=="__main__":
    AddAnnotationText()
