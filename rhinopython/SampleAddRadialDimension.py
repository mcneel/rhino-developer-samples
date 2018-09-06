################################################################################
# SampleAddRadialDimension.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

# Demonstrates how to create a radial dimension
def SampleAddRadialDimension():
    
    plane = Rhino.Geometry.Plane.WorldXY
    cp = Rhino.Geometry.Point3d(2.0, 2.0, 0.0)
    p1 = Rhino.Geometry.Point3d(4.0, 4.0, 0.0)
    p2 = Rhino.Geometry.Point3d(8.0, 6.0, 0.0)
    
    style = sc.doc.DimStyles.Current
    if (style.LeaderContentAngleType == Rhino.DocObjects.DimensionStyle.LeaderContentAngleStyle.Aligned):
        p2.Y = 8.0
    
    dim = Rhino.Geometry.RadialDimension.Create(style, Rhino.Geometry.AnnotationType.Radius, plane, cp, p1, p2)
    if dim:
        sc.doc.Objects.Add(dim)
        sc.doc.Views.Redraw()
        
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleAddRadialDimension()