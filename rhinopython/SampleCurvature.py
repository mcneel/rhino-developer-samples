################################################################################
# SampleCurvature.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import System
import scriptcontext

# Rhino.Input.Custom.GetPoint derived class that 
# dynamically draws a curvature circle.   
class SampleGetCurvaturePoint(Rhino.Input.Custom.GetPoint):
    
    # Class initializer
    def __init__(self, curve):
        self.m_curve = curve
        self.Constrain(curve, False)
    
    # Calculates the curvature
    def CalculateCurvature(self, point):
        rc, t = self.m_curve.ClosestPoint(point)
        if rc:
            tv = self.m_curve.TangentAt(t)
            if not tv.IsTiny():
                cv = self.m_curve.CurvatureAt(t)
                k = cv.Length
                if (k > Rhino.RhinoMath.SqrtEpsilon):
                    rv = cv / (k * k)
                    circle = Rhino.Geometry.Circle(point, tv, point + 2.0*rv)
                    return circle
        return None
    
    # OnMouseMove override
    def OnMouseMove(self, e):
        self.m_circle = self.CalculateCurvature(e.Point)
        GetPoint.OnMouseMove(self, e)
    
    # OnDynamicDraw override
    def OnDynamicDraw(self, e):
        if (self.m_circle):
            e.Display.DrawCircle(self.m_circle, System.Drawing.Color.Black)
        GetPoint.OnDynamicDraw(self, e)


# Draws a curvature circle.   
def SampleCurvature():
    
    # Select curve for curvature measurement
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select curve for curvature measurement")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.SubObjectSelect = False
    go.Get()
    if go.CommandResult() <> Rhino.Commands.Result.Success:
        return go.CommandResult()
    
    curve = go.Object(0).Curve()
    if not curve:
        return Rhino.Commands.Result.Failure
    
    # Select point on curve for curvature measurement
    gp = SampleGetCurvaturePoint(curve)
    gp.SetCommandPrompt("Select point on curve for curvature measurement")
    gp.Get()
    if gp.CommandResult() <> Rhino.Commands.Result.Success:
        return gp.CommandResult()
    
    # One final curvature calculation
    point = gp.Point()
    circle = gp.CalculateCurvature(point)
    if (circle):
        scriptcontext.doc.Objects.AddPoint(point)
        scriptcontext.doc.Objects.AddCircle(circle)
        scriptcontext.doc.Views.Redraw();
    
    return Rhino.Commands.Result.Success

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleCurvature()
