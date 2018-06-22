################################################################################
# SampleCreateFromLoftRebuild.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext
import System

# Creates a lofted surface
def SampleCreateFromLoftRebuild():
    
    p0 = Rhino.Geometry.Point3d(0,0,0)
    p1 = Rhino.Geometry.Point3d(0,10,0)
    l0 = Rhino.Geometry.LineCurve(p0, p1)    
    
    p0 = Rhino.Geometry.Point3d(5,0,5)
    p1 = Rhino.Geometry.Point3d(5,10,5)
    l1 = Rhino.Geometry.LineCurve(p0, p1)    
    
    p0 = Rhino.Geometry.Point3d(10,0,0)
    p1 = Rhino.Geometry.Point3d(10,10,0)
    l2 = Rhino.Geometry.LineCurve(p0, p1)    
    
    crvs = System.Array[Rhino.Geometry.Curve]([l0, l1, l2])
    unset = Rhino.Geometry.Point3d.Unset
    ltype = Rhino.Geometry.LoftType.Normal
    breps = Rhino.Geometry.Brep.CreateFromLoftRebuild(crvs, unset, unset, ltype, False, 8)
    if breps:
        for b in breps:
            scriptcontext.doc.Objects.AddBrep(b)
        scriptcontext.doc.Views.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleCreateFromLoftRebuild()
