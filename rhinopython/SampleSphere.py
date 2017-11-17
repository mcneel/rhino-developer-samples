################################################################################
# SampleSphere.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

# Rhino.Input.Custom.GetPoint derived class that dynamically
# draws a sphere.   
class SampleGetSphere(Rhino.Input.Custom.GetPoint):
    
    # Class initializer
    def __init__(self, plane):
        self.m_plane = plane
        self.m_sphere = Rhino.Geometry.Sphere(plane, 1.0)
        self.m_draw = False
        self.m_color = Rhino.ApplicationSettings.AppearanceSettings.TrackingColor
    
    # Calculates the sphere
    def CalculateSphere(self, point):
        rc = False
        dir = point - self.m_plane.Origin
        if (not dir.IsTiny()):
            self.m_sphere.Radius = dir.Length
            rc = True
        return rc
    
    # OnMouseMove override
    def OnMouseMove(self, e):
        self.m_draw = self.CalculateSphere(e.Point)
        # Call base class...
        Rhino.Input.Custom.GetPoint.OnMouseMove(self, e)
    
    # OnDynamicDraw override
    def OnDynamicDraw(self, e):
        if self.m_draw:
            e.Display.DrawSphere(self.m_sphere, self.m_color)
        # Call base class...
        Rhino.Input.Custom.GetPoint.OnDynamicDraw(self, e)
        
    # Returns the calculated sphere
    def Sphere(self):
        return self.m_sphere
    
    
def SampleSphere():
    
    # Center of sphere
    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Center of sphere")
    gp.Get()
    if gp.CommandResult() <> Rhino.Commands.Result.Success:
        return gp.CommandResult()
    
    # Calculate base plane
    base_plane = gp.View().ActiveViewport.ConstructionPlane()
    base_plane.Origin = gp.Point()
    
    # Radius
    gs = SampleGetSphere(base_plane)
    gs.SetCommandPrompt("Radius")
    gs.SetBasePoint(base_plane.Origin, True)
    gs.DrawLineFromPoint(base_plane.Origin, True)
    gs.Get()
    if gs.CommandResult() <> Rhino.Commands.Result.Success:
        return gs.CommandResult()
    
    # One final calculation
    if gs.CalculateSphere(gs.Point()):
        sc.doc.Objects.AddSphere(gs.Sphere())
        sc.doc.Views.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleSphere()
