################################################################################
# SamplePointCloudColors.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import System
import scriptcontext as sc

# Generates a random point on a circle
def RandomCirclePoint(center, radius, rnd):
    angle = 2.0 * System.Math.PI * rnd.NextDouble()
    x = center.X + radius * System.Math.Cos(angle)
    y = center.Y + radius * System.Math.Sin(angle)
    return Rhino.Geometry.Point3d(x, y, 0.0)

# Generates a random color
def RandomColor(rnd):
    return System.Drawing.Color.FromArgb(rnd.Next(256), rnd.Next(256), rnd.Next(256))
    
# Creates a point cloud with colored vertices.
def SamplePointCloudColors():
    rnd = System.Random()
    pc = Rhino.Geometry.PointCloud()
    
    center = Rhino.Geometry.Point3d.Origin
    radius = 4.0
    
    count = 100
    i = 0
    
    while i < count:
        pt = RandomCirclePoint(center, radius, rnd)
        clr = RandomColor(rnd)
        pc.Add(pt, clr)
        i += 1
    
    sc.doc.Objects.AddPointCloud(pc)
    sc.doc.Views.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SamplePointCloudColors()