################################################################################
# SampleSetCPlaneToView.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

# Sets the construction plane in the active viewport parallel to the view.
def SampleSetCPlaneToView():
    view = sc.doc.Views.ActiveView
    if view:
        vp = view.ActiveViewport
        target = vp.CameraTarget
        cam_x = vp.CameraX
        cam_y = vp.CameraY
        cplane = vp.GetConstructionPlane()
        cplane.Plane = Rhino.Geometry.Plane(target, cam_x, cam_y)
        view.ActiveViewport.PushConstructionPlane(cplane)
        view.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleSetCPlaneToView()