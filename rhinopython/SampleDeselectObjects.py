################################################################################
# SampleDeselectObjects.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino

# Demonstrates how to get seleced objects and allow the user to deselect them.
def SampleDeselectObjects():
    
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt('Select objects')
    go.EnablePreSelect(True, True)
    go.EnablePostSelect(False)
    go.SubObjectSelect = False
    go.GetMultiple(1, 0)
    if (go.CommandResult() != Rhino.Commands.Result.Success or 0 == go.ObjectCount):
        Rhino.RhinoApp.WriteLine("No pre-selected objects.");
        return None
    
    go.DisablePreSelect()
    go.EnablePostSelect(True)
    go.AlreadySelectedObjectSelect = True
    go.DeselectAllBeforePostSelect = False
    go.EnableClearObjectsOnEntry(False)
    go.EnableUnselectObjectsOnExit(False)
    go.GetMultiple(1, 0)
    if (go.CommandResult() != Rhino.Commands.Result.Success):
        return None
    
    # Normally, pre-selected objects will remain selected, when a
    # command finishes, and post-selected objects will be unselected.
    # This this way of picking, it is possible to have a combination
    # of pre-selected and post-selected. So, to make sure everything
    # "looks the same", lets select everything before finishing.
    rc = []
    for i in range(go.ObjectCount):
        obj = go.Object(i).Object()
        if obj:
            obj.Select(True)
            rc.append(obj.Id)
    return rc

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    rc = SampleDeselectObjects()
    if rc:
        print rc


