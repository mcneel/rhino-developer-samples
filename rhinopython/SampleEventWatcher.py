################################################################################
# SampleEventHandler.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

################################################################################
# SampleEventHandler class
################################################################################
class SampleEventHandler():
    
    # Initializer
    def __init__(self):
        # Enable the event handlers
        Rhino.RhinoDoc.AddRhinoObject += self.OnAddRhinoObject
        Rhino.RhinoDoc.ReplaceRhinoObject += self.OnReplaceRhinoObject
        Rhino.RhinoDoc.DeleteRhinoObject += self.OnDeleteRhinoObject
        Rhino.RhinoDoc.UndeleteRhinoObject += self.OnUndeleteRhinoObject
    
    # Disables the event handlers
    def Disable(self):
        Rhino.RhinoDoc.AddRhinoObject -= self.OnAddRhinoObject
        Rhino.RhinoDoc.ReplaceRhinoObject -= self.OnReplaceRhinoObject
        Rhino.RhinoDoc.DeleteRhinoObject -= self.OnDeleteRhinoObject
        Rhino.RhinoDoc.UndeleteRhinoObject -= self.OnUndeleteRhinoObject
    
    # AddRhinoObject event handler
    def OnAddRhinoObject(self, sender, e):
        print '> SampleEventHandler.OnAddRhinoObject'
    
    # ReplaceRhinoObject event handler
    def OnReplaceRhinoObject(self, sender, e):
        print '> SampleEventHandler.OnReplaceRhinoObject'
    
    # DeleteRhinoObject event handler
    def OnDeleteRhinoObject(self, sender, e):
        print '> SampleEventHandler.OnDeleteRhinoObject'
    
    # UndeleteRhinoObject event handler
    def OnUndeleteRhinoObject(self, sender, e):
        print '> SampleEventHandler.OnUndeleteRhinoObject'      
        
################################################################################
# TestSampleEventHandler function
################################################################################
def TestSampleEventHandler():
    # See if we already have a handler
    if sc.sticky.has_key('sample_event_handler'):
        # Delete the handler
        handler = sc.sticky.pop('sample_event_handler', None)
        if handler:
            handler.Disable()
            handler = None
            print ' SampleEventHandler disabled'  
    else:
        # Create handler
        handler = SampleEventHandler()
        # Add the handler to the sticky dictionary so it
        # survives when the main function ends.
        sc.sticky['sample_event_handler'] = handler       
        print ' SampleEventHandler enabled'  
    
################################################################################
# Check to see if this file is being executed as the 'main' python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == '__main__':
    TestSampleEventHandler()    