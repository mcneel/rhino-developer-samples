################################################################################
# SampleMultiplePointsConduit.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

import System
import Rhino
import scriptcontext as sc

################################################################################
# SampleMultiplePointsConduit display conduit class
################################################################################
class SampleMultiplePointsConduit(Rhino.Display.DisplayConduit):
    
    # Initializer
    def __init__(self):
        self.m_points = System.Collections.Generic.List[Rhino.Geometry.Point3d]()
        self.m_color = Rhino.ApplicationSettings.AppearanceSettings.DefaultObjectColor
        self.m_point_style = Rhino.Display.PointStyle.ControlPoint
    
    # CalculateBoundingBox override
    def CalculateBoundingBox(self, e):
        bbox = Rhino.Geometry.BoundingBox(self.m_points)
        e.IncludeBoundingBox(bbox)
    
    # DrawOverlay override
    def DrawOverlay(self, e):
        e.Display.DrawPoints(self.m_points, self.m_point_style, 3, self.m_color)
    
    # Gets the points
    @property
    def Points(self):
        return self.m_points
        
    # Gets the point count
    @property
    def PointCount(self):
        return self.m_points.Count
    
    # Adds a point
    def AddPoint(self, pt):
        self.m_points.Add(pt)
    
    # Removes the last point
    def RemovePoint(self):
        if self.m_points.Count > 0:
            self.m_points.RemoveAt(self.m_points.Count - 1)

################################################################################
# TestSampleMultiplePointsConduit function
################################################################################
def TestSampleMultiplePointsConduit():
          
    conduit = SampleMultiplePointsConduit()
    conduit.Enabled = True
    rc = Rhino.Commands.Result.Success
    
    gp = Rhino.Input.Custom.GetPoint()
    while (True):
        if (0 == conduit.PointCount):
            gp.SetCommandPrompt("Location of point object.")
            gp.AcceptNothing(False)
            gp.AcceptUndo(False)
        else:
            gp.SetCommandPrompt("Location of point object. Press Enter when done")
            gp.AcceptNothing(False)
            gp.AcceptUndo(False)
        
        res = gp.Get()
        
        if res == Rhino.Input.GetResult.Point:
            conduit.AddPoint(gp.Point())
            sc.doc.Views.Redraw()
        elif res == Rhino.Input.GetResult.Undo:
            conduit.RemovePoint()
            sc.doc.Views.Redraw()
        elif res == Rhino.Input.GetResult.Nothing:
            break
        else:
            rc = Rhino.Commands.Result.Cancel
            break
        
        if (rc == Rhino.Commands.Result.Success and conduit.PointCount > 0):
            sc.doc.Objects.AddPoints(conduit.Points)
        
        conduit.Enabled = False
        sc.doc.Views.Redraw()

################################################################################
# Check to see if this file is being executed as the 'main' python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == '__main__':
    TestSampleMultiplePointsConduit()