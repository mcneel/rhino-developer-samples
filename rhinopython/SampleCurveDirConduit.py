################################################################################
# SampleCurveDirConduit.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

import System
import Rhino
import scriptcontext as sc

################################################################################
# SampleCurveDirConduit display conduit class
################################################################################
class SampleCurveDirConduit(Rhino.Display.DisplayConduit):
    
    # Initializer
    def __init__(self):
        self.m_curve = None
        self.m_arrow_count = 3
    
    # DrawOverlay override
    def DrawOverlay(self, e):
        if self.m_curve:
            color = Rhino.ApplicationSettings.AppearanceSettings.TrackingColor
            for x in range(0, self.m_arrow_count):
                d = x / (self.m_arrow_count - 1)
                t = self.m_curve.Domain.ParameterAt(d)
                pt = self.m_curve.PointAt(t)
                dir = self.m_curve.TangentAt(t)
                e.Display.DrawDirectionArrow(pt, dir, color)
    
    @property
    def Curve(self):
        return self.m_curve
    
    @Curve.setter
    def Curve(self, val):
        self.m_curve = val
    
################################################################################
# TestSampleCurveDirConduit function
################################################################################
def TestSampleCurveDirConduit():
          
    res, objref = Rhino.Input.RhinoGet.GetOneObject(
        "Select curve for direction display", 
        True, 
        Rhino.DocObjects.ObjectType.Curve
        )
    if res != Rhino.Commands.Result.Success: 
        return
    
    curve = objref.Curve()
    if curve is None: 
        return
    
    conduit = SampleCurveDirConduit()
    conduit.Curve = curve
    conduit.Enabled = True
    sc.doc.Views.Redraw()
    
    out_str = ""
    rc = Rhino.Input.RhinoGet.GetString("Press <Enter> to continue", True, out_str);
    conduit.Enabled = False
    sc.doc.Views.Redraw()
    
################################################################################
# Check to see if this file is being executed as the 'main' python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == '__main__':
    TestSampleCurveDirConduit()