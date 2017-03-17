################################################################################
# SampleSetCameraTarget.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

# Demonstrates how to set a viewport's camera and target locations.
def SampleSetCameraTarget():
    
    view = sc.doc.Views.ActiveView
    vp = view.ActiveViewport
      
    target = vp.CameraTarget
    camera = vp.CameraLocation

    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt('New target location')
    gp.SetDefaultPoint(target)
    gp.Get()
    if (gp.CommandResult() != Rhino.Commands.Result.Success):
        return;
    target = gp.Point()
    
    gp.ClearDefault()
    gp.SetCommandPrompt('New camera location')
    gp.SetDefaultPoint(camera)
    gp.SetBasePoint(target, True)
    gp.DrawLineFromPoint(target, True)
    gp.Get()
    if (gp.CommandResult() != Rhino.Commands.Result.Success):
        return;
    camera = gp.Point()
        
    camdir = target - camera;
    camdir.Unitize()
    if (camdir.IsTiny()):
        return;
        
    vp.SetCameraLocations(target, camera)
    view.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleSetCameraTarget()
