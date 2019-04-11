################################################################################
# SampleGetPolyline.py
# Copyright (c) 20197 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import System
import Rhino
import scriptcontext as sc

# GetPoint-derived class
class GetPolylinePoint(Rhino.Input.Custom.GetPoint):
    
    def __init__(self):
        self.color = Rhino.ApplicationSettings.AppearanceSettings.DefaultLayerColor
        self.points = []
    
    def Count(self):
        return len(self.points)
        
    def PointArray(self):
        return System.Array[Rhino.Geometry.Point3d](self.points)
        
    def PointFromIndex(self, index):
        return self.points[index]
        
    def AppendPoint(self, point):
        self.points.append(point)
    
    def OnDynamicDraw(self, e):
        point_count = len(self.points)
        if point_count > 1:
            for i in range(1, point_count):
                e.Display.DrawLine(self.points[i-1], self.points[i], self.color)
            for i in range(point_count-1):
                e.Display.DrawPoint(self.points[i])
        Rhino.Input.Custom.GetPoint.OnDynamicDraw(self, e)

# Function to pick and add a polyline to the document.
# Note, this function isn't nearly as fancy as RhinoCommon's
# Rhino.Input.Custom.GetPolyline.
def SampleGetPolyline():
    
    got_pline = False
    
    gp = GetPolylinePoint()
    while not got_pline:
        gp.ClearConstructionPoints()
        gp.ClearSnapPoints()
        gp.ConstrainDistanceFromBasePoint(Rhino.RhinoMath.UnsetValue)
        gp.PermitFromOption(True)
        
        point_count = gp.Count()
        if point_count > 0:
            gp.AddSnapPoints(gp.PointArray())
            gp.SetBasePoint(gp.PointFromIndex(-1), False)
        if point_count >= 3:
            gp.AddConstructionPoint(gp.PointFromIndex(0))
            
        if point_count == 0:
            gp.SetCommandPrompt("Start of polyline")
            gp.AcceptNothing(False)
            gp.SetBasePoint(Rhino.Geometry.Point3d.Unset, False)
            gp.EnableDrawLineFromPoint(False)   
        else:
            if point_count >= 2:
                gp.SetCommandPrompt("Next point of polyline. Press Enter when done")
                gp.AcceptNothing(True)
            else:
                gp.SetCommandPrompt("Next point of polyline")
            gp.SetBasePoint(Rhino.Geometry.Point3d.Unset, False)
            gp.DrawLineFromPoint(gp.PointFromIndex(-1), True)
        
        gp.EnableNoRedrawOnExit(True)
        res = gp.Get()
        
        if res == Rhino.Input.GetResult.Nothing:
            got_pline = True
            break
            
        if res != Rhino.Input.GetResult.Point:
            break
            
        if gp.Count() == 0:
            gp.AppendPoint(gp.Point())
        else:
            p = gp.PointFromIndex(-1)
            if p.DistanceTo(gp.Point()) >= Rhino.RhinoMath.SqrtEpsilon:
                gp.AppendPoint(gp.Point())
    
    if got_pline:
        sc.doc.Objects.AddPolyline(gp.PointArray())
    
    sc.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleGetPolyline()