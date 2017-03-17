################################################################################
# SampleCircleRadius.py -- March 2016
# Copyright (c) Robert McNeel & Associates
################################################################################
import Rhino
import scriptcontext
import System.Drawing.Color

# Rhino.Input.Custom.GetPoint derived class 
# that dynamically draws a circle.   
class GetRadiusPoint(Rhino.Input.Custom.GetPoint):
    
    # Class initializer
    def __init__(self, plane):
        self.base_plane = plane
        self.can_draw = False
        self.draw_color = Rhino.ApplicationSettings.AppearanceSettings.DefaultLayerColor
        self.circle = Rhino.Geometry.Circle(1.0)
        self.SetBasePoint(plane.Origin, True)
        self.DrawLineFromPoint(plane.Origin, True)
        self.Constrain(plane, False)
    
    # OnMouseMove override
    def OnMouseMove(self, e):
        self.can_draw = self.CalculateCircle(e.Point)
        Rhino.Input.Custom.GetPoint.OnMouseMove(self, e)
    
    # OnDynamicDraw override
    def OnDynamicDraw(self, e):
        if (self.can_draw):
            e.Display.DrawCircle(self.circle, self.draw_color)
        Rhino.Input.Custom.GetPoint.OnDynamicDraw(self, e)
    
    # Calculate the circle
    def CalculateCircle(self, test_point):
        rc = False
        point = self.base_plane.ClosestPoint(test_point)
        dir = point - self.base_plane.Origin
        if (not dir.IsTiny()):
            self.circle = Rhino.Geometry.Circle(self.base_plane, dir.Length)
            rc = self.circle.IsValid
        return rc
    
    #Return the circle
    def Circle(self):
        return self.circle


# Creates a circle
def SampleCircleRadius():
    
    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt('Center of circle')
    gp.Get();
    if (gp.CommandResult() != Rhino.Commands.Result.Success):
        return gp.CommandResult()
    
    view = gp.View()
    if not view:
        return Rhino.Commands.Result.Failure
    
    plane = view.ActiveViewport.ConstructionPlane()
    plane.Origin = gp.Point()
    
    gr = GetRadiusPoint(plane);
    gr.SetCommandPrompt('Radius')
    gr.Get()
    if (gr.CommandResult() != Rhino.Commands.Result.Success):
        return gr.CommandResult()

    rc = Rhino.Commands.Result.Failure
    if (gr.CalculateCircle(gr.Point())):
        scriptcontext.doc.Objects.AddCircle(gr.Circle())
        scriptcontext.doc.Views.Redraw()
        rc = Rhino.Commands.Result.Success
    
    return rc;


# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if( __name__ == '__main__' ):
    SampleCircleRadius()
