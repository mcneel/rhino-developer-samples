################################################################################
# SampleDrawBrepEdges.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import scriptcontext
import rhinoscriptsyntax as rs
import Rhino
import System

# Demonstrates how to draw the edges of selected Breps with
# a special color.
class SampleDrawBrepEdgesConduit(Rhino.Display.DisplayConduit):
    # class members
    draw_color = System.Drawing.Color.Pink
    # class constructor
    def __init__(self, brep):
        self.brep = brep
    # DrawOverlay override
    def DrawOverlay(self, e):
        for edge in self.brep.Edges:
            e.Display.DrawCurve(edge, self.draw_color, 3)

def SampleDrawBrepEdges():
    # Pick a Brep
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt('Select Brep')
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Brep
    go.Get()
    if (go.CommandResult() == Rhino.Commands.Result.Success):
        # Get some stuff...
        obj = go.Object(0).Object()    
        brep = go.Object(0).Brep()
        if obj and brep:
            # Unselect the object
            obj.Select(False)
            # Create and enable the conduit
            conduit = SampleDrawBrepEdgesConduit(brep)
            conduit.Enabled = True
            scriptcontext.doc.Views.Redraw()
            # Wait for the user...
            rs.GetString('Press <Enter> to continue')
            # Disable the conduit
            conduit.Enabled = False
            scriptcontext.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleDrawBrepEdges()
