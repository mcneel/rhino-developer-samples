import Rhino
import scriptcontext
import System.Guid, System.Drawing.Color

def SelLayer():
    # Prompt for a layer name
    layername = scriptcontext.doc.Layers.CurrentLayer.Name
    rc, layername = Rhino.Input.RhinoGet.GetString("Name of layer to select objects", True, layername)
    if rc!=Rhino.Commands.Result.Success: return rc

    # Get all of the objects on the layer. If layername is bogus, you will
    # just get an empty list back
    rhobjs = scriptcontext.doc.Objects.FindByLayer(layername)
    if not rhobjs: Rhino.Commands.Result.Cancel

    for obj in rhobjs: obj.Select(True)
    scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    SelLayer()
