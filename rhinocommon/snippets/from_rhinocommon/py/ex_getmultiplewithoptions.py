import Rhino
import scriptcontext

def GetMultipleWithOptions():
    geometryFilter = Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.PolysrfFilter | Rhino.DocObjects.ObjectType.Mesh
    integer1 = 300
    integer2 = 300
    
    optionInteger1 = Rhino.Input.Custom.OptionInteger(integer1, 200, 900)
    optionInteger2 = Rhino.Input.Custom.OptionInteger(integer2, 200, 900)
    
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select surfaces, polysurfaces, or meshes")
    go.GeometryFilter = geometryFilter
    go.AddOptionInteger("Option1", optionInteger1)
    go.AddOptionInteger("Option2", optionInteger2)
    go.GroupSelect = True
    go.SubObjectSelect = False
    go.EnableClearObjectsOnEntry(False)
    go.EnableUnselectObjectsOnExit(False)
    go.DeselectAllBeforePostSelect = False
    
    bHavePreselectedObjects = False
    
    while True:
        res = go.GetMultiple(1, 0)
        
        if res == Rhino.Input.GetResult.Option:
            go.EnablePreSelect(False, True)
            continue
        
        elif res != Rhino.Input.GetResult.Object:
            return Rhino.Commands.Result.Cancel
        
        if go.ObjectsWerePreselected:
            bHavePreselectedObjects = True
            go.EnablePreSelect(False, True)
            continue
        
        break
    
    if bHavePreselectedObjects:
        # Normally, pre-selected objects will remain selected, when a
        # command finishes, and post-selected objects will be unselected.
        # This this way of picking, it is possible to have a combination
        # of pre-selected and post-selected. So, to make sure everything
        # "looks the same", lets unselect everything before finishing
        # the command.
        for i in range(0, go.ObjectCount):
            rhinoObject = go.Object(i).Object()
            if not rhinoObject is None:
                rhinoObject.Select(False)
        scriptcontext.doc.Views.Redraw()
    
    objectCount = go.ObjectCount
    integer1 = optionInteger1.CurrentValue
    integer2 = optionInteger2.CurrentValue
    
    Rhino.RhinoApp.WriteLine("Select object count = {0}", objectCount)
    Rhino.RhinoApp.WriteLine("Value of integer1 = {0}", integer1)
    Rhino.RhinoApp.WriteLine("Value of integer2 = {0}", integer2)
    
    return Rhino.Commands.Result.Success
    
if __name__ == "__main__":
    GetMultipleWithOptions()