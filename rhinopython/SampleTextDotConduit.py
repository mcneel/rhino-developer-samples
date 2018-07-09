################################################################################
# SampleTextDotConduit.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import System
import Rhino
import scriptcontext

class SampleTextDotConduit(Rhino.Display.DisplayConduit):
    
    # Initializer
    def __init__(self, uuid, text):
        # Initialize member variables
        self.m_uuid = uuid
        self.m_text = text
        self.m_dirty = False
        self.Start()
    
    def Start(self):
        # Create event handlers
        self.CreateEvents()
        # Enable conduit
        self.Enabled = True
    
    def Stop(self):
        # Remove event handlers
        self.RemoveEvents()
        # Disable conduit
        self.Enabled = False
    
    # Text property
    @property
    def Text(self):
        return self.m_text
    @Text.setter
    def Text(self, val):
        self.m_text = val
        self.m_dirty = True
    
    # GetBoundingBox
    def GetBoundingBox(self):
        unset = Rhino.Geometry.Point3d.Unset
        bbox = Rhino.Geometry.BoundingBox(unset, unset)
        rh_obj = scriptcontext.doc.Objects.FindId(self.m_uuid)
        if rh_obj:
            bbox = rh_obj.Geometry.GetBoundingBox(True)
        return bbox
    
    # DisplayConduit.CalculateBoundingBox override
    def CalculateBoundingBox(self, e):
        bbox = self.GetBoundingBox()
        if bbox.IsValid:
            e.IncludeBoundingBox(bbox)
    
    # DisplayConduit.PostDrawObjects override
    def DrawOverlay(self, e):
        if self.m_text:
            bbox = self.GetBoundingBox()
            if bbox.IsValid:
                e.Display.DrawDot(bbox.Center, self.m_text)
    
    # Create Rhino event handlers
    def CreateEvents(self):
        Rhino.RhinoDoc.CloseDocument += self.OnCloseDocument
        Rhino.RhinoDoc.ReplaceRhinoObject += self.OnReplaceObject
        Rhino.RhinoApp.Idle += self.OnIdle
        
    # Remove Rhino event handlers
    def RemoveEvents(self):        
        Rhino.RhinoDoc.CloseDocument -= self.OnCloseDocument
        Rhino.RhinoDoc.ReplaceRhinoObject -= self.OnReplaceObject
        Rhino.RhinoApp.Idle -= self.OnIdle
    
    # RhinoDoc.CloseDocument event handler
    def OnCloseDocument(self, sender, e):
        self.Stop()
        self.m_uuid = None
        self.m_text = None
    
    # RhinoDoc.ReplaceRhinoObject event handler
    def OnReplaceObject(self, sender, e):
        if e.ObjectId == self.m_uuid:
            self.m_dirty = True
    
    # RhinoApp.Idle event handler
    def OnIdle(self, sender, e):
        if self.m_dirty == True:
            self.m_dirty = False
    
    def IsValid(self):
        if self.m_uuid and self.m_text:
            return True
        return False
    
################################################################################
# Tests the conduit.
################################################################################    
def TestSampleTextDotConduit():
    
    # See if the form is already visible
    if scriptcontext.sticky.has_key('sample_textdot_conduit'):
        conduit = scriptcontext.sticky['sample_textdot_conduit']
        if conduit and conduit.IsValid():
            # Prompt for some text
            default = ''
            rc, text = Rhino.Input.RhinoGet.GetString("Text to display", True, default)
            if (rc == Rhino.Commands.Result.Success):
                # Update the text
                conduit.Text = text
            else:
                # Stop the conduit and event watchers
                conduit.Stop()
                # Destroy class object
                conduit = None
                # Remove sticky
                scriptcontext.sticky.Remove('sample_textdot_conduit')
                
            # Redraw
            scriptcontext.doc.Views.Redraw()
            return
            
    if scriptcontext.sticky.has_key('sample_textdot_conduit'):
        conduit = scriptcontext.sticky['sample_textdot_conduit']
        if conduit:
            conduit = None
                
    # Select an object
    filter = Rhino.DocObjects.ObjectType.AnyObject
    rc, objref = Rhino.Input.RhinoGet.GetOneObject('Select object', False, filter)
    if (rc != Rhino.Commands.Result.Success):
        return
    
    # Prompt for some text
    default = 'Test'
    rc, text = Rhino.Input.RhinoGet.GetString("Text to display", False, default)
    if (rc != Rhino.Commands.Result.Success or not text):
        return
    
    # Create an instance of our class
    conduit = SampleTextDotConduit(objref.ObjectId, text)
    
    # Add the sticky dictionary
    scriptcontext.sticky['sample_textdot_conduit'] = conduit
    
    # Redraw
    scriptcontext.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.    
if __name__ == '__main__':
    TestSampleTextDotConduit()