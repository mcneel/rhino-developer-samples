import Rhino
import scriptcontext

def ObjectDisplayMode():
    filter = Rhino.DocObjects.ObjectType.Brep | Rhino.DocObjects.ObjectType.Mesh
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select mesh or surface", True, filter)
    if rc != Rhino.Commands.Result.Success: return rc;
    viewportId = scriptcontext.doc.Views.ActiveView.ActiveViewportID

    attr = objref.Object().Attributes
    if attr.HasDisplayModeOverride(viewportId):
        print "Removing display mode override from object"
        attr.RemoveDisplayModeOverride(viewportId)
    else:
        modes = Rhino.Display.DisplayModeDescription.GetDisplayModes()
        mode = None
        if len(modes) == 1:
            mode = modes[0]
        else:
            go = Rhino.Input.Custom.GetOption()
            go.SetCommandPrompt("Select display mode")
            str_modes = []
            for m in modes:
                s = m.EnglishName.replace(" ","").replace("-","")
                str_modes.append(s)
            go.AddOptionList("DisplayMode", str_modes, 0)
            if go.Get()==Rhino.Input.GetResult.Option:
                mode = modes[go.Option().CurrentListOptionIndex]
        if not mode: return Rhino.Commands.Result.Cancel
        attr.SetDisplayModeOverride(mode, viewportId)
    scriptcontext.doc.Objects.ModifyAttributes(objref, attr, False)
    scriptcontext.doc.Views.Redraw()


if __name__=="__main__":
    ObjectDisplayMode()
