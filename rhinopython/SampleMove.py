################################################################################
# SampleMove.py -- September 2014
# Copyright (c) Robert McNeel & Associates
################################################################################
from Rhino import *
from Rhino.Commands import *
from Rhino.Geometry import *
from Rhino.Input.Custom import *
from scriptcontext import doc

# Rhino.Input.Custom.GetPoint derived class that dynamically
# draws the selected objects.   
class SampleGetMovePoint(GetPoint):
    
    # Class initializer
    def __init__(self, go, base_point):
        self.m_objects = [o.Object() for o in go.Objects()]
        self.m_point = base_point
        self.m_xform = Transform.Identity
    
    # Calculates the transformation
    def CalculateTransform(self, point):
        xform = Transform.Identity
        dir = point - self.m_point
        if (not dir.IsTiny()):
            xform = Transform.Translation(dir)
        return xform
    
    # OnMouseMove override
    def OnMouseMove(self, e):
        self.m_xform = self.CalculateTransform(e.Point)
        GetPoint.OnMouseMove(self, e)
    
    # OnDynamicDraw override
    def OnDynamicDraw(self, e):
        for obj in self.m_objects:
            e.Display.DrawObject(obj, self.m_xform)
        GetPoint.OnDynamicDraw(self, e)
        
        
# Moves objects from one location to another.   
def SampleMove():
    
    # Select objects to move
    go = GetObject()
    go.SetCommandPrompt("Select objects to move")
    go.GetMultiple(1, 0)
    if go.CommandResult() <> Result.Success:
        return go.CommandResult()
    
    # Point to move from
    gp = GetPoint()
    gp.SetCommandPrompt("Point to move from")
    gp.Get()
    if gp.CommandResult() <> Result.Success:
        return gp.CommandResult()
    
    base_point = gp.Point()
    
    # Point to move to
    gm = SampleGetMovePoint(go, base_point)
    gm.SetCommandPrompt("Point to move to")
    gm.SetBasePoint(base_point, True)
    gm.DrawLineFromPoint(base_point, True)
    gm.Get()
    if gm.CommandResult() <> Result.Success:
        return gm.CommandResult()
    
    target_point = gm.Point()
    
    # One final transformation calculation
    xform = gm.CalculateTransform(target_point);

    # Transform (move) the objects
    for o in go.Objects():
        doc.Objects.Transform(o, xform, True);
        doc.Views.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleMove()
