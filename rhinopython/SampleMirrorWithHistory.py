################################################################################
# SampleMirrorWithHistory.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import rhinoscriptsyntax
import scriptcontext

# Ensures the Mirror command is run with history recording enabled
def SampleMirrorWithHistory():
    
    # select object to mirror
    object_ids = rhinoscriptsyntax.GetObjects("Select object to mirror", 0, True, True, True)
    if not object_ids:
        return
        
    # restore default axis if it has been saved
    default = False
    if scriptcontext.sticky.has_key("axis"):
        default = scriptcontext.sticky["axis"]
        
    # specify mirror axis
    items = ("Axis", "Xaxis", "Yaxis")
    rc = rhinoscriptsyntax.GetBoolean("Mirror axis", items, [default])
    if not rc:
        return
        
    # setup scriptable option
    axis = " _Xaxis "
    if rc[0] == True:
        axis = " _Yaxis "    
        
    # disable redraw (for speed)
    rhinoscriptsyntax.EnableRedraw(False)
    # get current state of history recording
    state = Rhino.ApplicationSettings.HistorySettings.RecordingEnabled
    # turn on history recording
    Rhino.ApplicationSettings.HistorySettings.RecordingEnabled = True
    # run the Mirror command
    rhinoscriptsyntax.Command("_-Mirror" + axis + "_Enter", True)
    # restore state of history recording
    Rhino.ApplicationSettings.HistorySettings.RecordingEnabled = state
    # save the axis value for use in the future
    scriptcontext.sticky["axis"] = rc
    # enable redraw
    rhinoscriptsyntax.EnableRedraw(True)
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":    
    SampleMirrorWithHistory()    