################################################################################
# SampleGetArc.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

# Rhino.Input.Custom.GetObject derived class that limits picking to arcs. 
class SampleGetArcCurveObject(Rhino.Input.Custom.GetObject):
    
    # GetObject.CustomGeometryFilter override
    def CustomGeometryFilter(self, rh_object, geometry, component_index):
        
        if not geometry:
            return False
        
        if not isinstance(geometry, Rhino.Geometry.Curve):
            return False
        
        arc = self.GetArcDefinition(geometry)
        if arc is None:
            return False
        
        return True
    
    # Retrieves the arc definition from a curve
    def GetArcDefinition(self, curve):
        
        if curve is None:
            return None
        
        # Is arc curve?
        if isinstance(curve, Rhino.Geometry.ArcCurve):
            if not curve.IsCompleteCircle:
                return curve.Arc
        
        # Is NURBS curve with arc form?
        if isinstance(curve, Rhino.Geometry.NurbsCurve):
            rc, arc = curve.TryGetArc()
            if not arc is None and not arc.IsCircle:
                return arc
        
        return None

# Sample that tests the GetArcCurveObject class
def SampleGetArc():
    
    go = SampleGetArcCurveObject()
    go.SetCommandPrompt('Select arc')
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.Get()
    if go.CommandResult() <> Rhino.Commands.Result.Success:
        return go.CommandResult()
        
    curve = go.Object(0).Curve()
    if curve is None:
        return Rhino.Commands.Result.Failure
        
    arc = go.GetArcDefinition(curve)
    if not arc is None:
        sc.doc.Objects.AddArc(arc)
        sc.doc.Views.Redraw()
        
    return Rhino.Commands.Result.Success

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleGetArc()