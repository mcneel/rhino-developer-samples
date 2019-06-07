################################################################################
# SampleMouseCallback.py
# Copyright (c) 2019 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import scriptcontext as sc
import Rhino

# Demonstrates how to use RhinoCommon's MouseCallback class
class MouseCallbackClass(Rhino.UI.MouseCallback):

    def OnMouseDown(self, e):
        print "OnMouseDown",e.Button
    
    def OnEndMouseDown(self,e):
        print "OnEndMouseDown", e.Button

    def OnMouseUp(self,e):
        print "OnMouseUp",e.Button
    
    def OnEndMouseUp(self,e):
        print "OnEndMouseUp",e.Button

    def OnMouseDoubleClick(self,e):
        print "OnMouseDoubleClick",e.Button
    
    def OnMouseMove(self,e):
        print "OnMouseMove"

    def OnEndMouseMove(self,e):
        print "OnEndMouseMove"

    def OnMouseEnter(self,e):
        print "OnMouseEnter", e.View.MainViewport.Name
    
    def OnMouseLeave(self,e):
        print "OnMouseLeave", e.View.MainViewport.Name
    
    def OnMouseHover(self,e):
        print "OnMouseHover"

# The 'main' function   
def mouse_event_helper_func():
    if sc.sticky.has_key('SampleMouseCallback'):
        callback = sc.sticky['SampleMouseCallback']
        if callback:
            callback.Enabled = False
            callback = None
            sc.sticky.Remove('SampleMouseCallback')
    else:
        callback = MouseCallbackClass()
        callback.Enabled = True
        sc.sticky['SampleMouseCallback'] = callback
        print "Listening for mouse events..."

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    mouse_event_helper_func()
