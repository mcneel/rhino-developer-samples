# inspired by Dale Fugier's post here: https://discourse.mcneel.com/t/button-mouse-event-again/74625/14

import scriptcontext as sc
import Rhino

######################
### EVENT HANDLERS ###
######################
class MouseCallbackClass(Rhino.UI.MouseCallback):
    """"""
    def OnMouseDown(self, e):
        print "OnMouseDown",e.Button
    
    """
    def OnEndMouseDown(self,e):
        print "OnEndMouseDown", e.Button
    """
    """"""
    def OnMouseUp(self,e):
        print "OnMouseUp",e.Button
    
    """
    def OnEndMouseUp(self,e):
        print "OnEndMouseUp",e.Button
    """
    """"""
    def OnMouseDoubleClick(self,e):
        print "OnMouseDoubleClick",e.Button
    
    """
    def OnMouseMove(self,e):
        print "OnMouseMove"
    """
    """
    def OnEndMouseMove(self,e):
        print "OnEndMouseMove"
    """
    """"""
    def OnMouseEnter(self,e):
        print "OnMouseEnter", e.View.MainViewport.Name
    
    """"""
    def OnMouseLeave(self,e):
        print "OnMouseLeave", e.View.MainViewport.Name
    
    """"""
    def OnMouseHover(self,e):
        print "OnMouseHover"
    

##############################
### EVENT HELPER FUNCTIONS ###
##############################

def mouse_event_helper_func():
    #Event trigger helper function
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
        #Rhino.RhinoApp.WriteLine("Click somewhere...")
        print "Listening for mouse events..."



if __name__ == "__main__":
    mouse_event_helper_func()
