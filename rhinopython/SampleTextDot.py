################################################################################
# SampleTextDot.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import rhinoscriptsyntax as rs

# Rhino.Input.Custom.GetPoint derived class that dynamically
# draws the text dot.   
class SampleGetTextDotPoint(Rhino.Input.Custom.GetPoint):
    
    # String formatting function
    def FormatPointString(self, point):
        x = point.X.ToString("0.00")
        y = point.Y.ToString("0.00")
        z = point.Z.ToString("0.00")
        str = "{0},{1},{2}".format(x, y, z)
        return str
    
    # OnDynamicDraw override
    def OnDynamicDraw(self, e):
        point = e.CurrentPoint
        str = self.FormatPointString(point)
        e.Display.DrawDot(point, str)
    
# Draws an annotation dot that stays parallel to and sizes with the view.
def SampleTextDot():
    gp = SampleGetTextDotPoint();
    gp.Get()
    if (gp.CommandResult() == Rhino.Commands.Result.Success):
        point = gp.Point()
        str = gp.FormatPointString(point)
        rs.AddTextDot(str, point)
        rs.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleTextDot()