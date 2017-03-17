################################################################################
# SamplePointsOnSphere.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import rhinoscriptsyntax as rs
from math import cos, sin, pi, sqrt

# Demonstrates how to place points on a sphere
def GetPointsEquiAngularlyDistancedOnSphere(numberOfPoints=45):
    dlong = pi * (3.0 - sqrt(5.0))
    dz =  2.0/numberOfPoints
    long = 0.0
    z = 1.0 - dz / 2.0
    ptsOnSphere =[]
    for k in range( 0, numberOfPoints): 
        r = sqrt(1.0-z*z)
        ptNew = (cos(long) * r, sin(long) * r, z)
        ptsOnSphere.append(ptNew)
        z = z - dz
        long = long + dlong
    return ptsOnSphere

def SamplePointsOnSphere():
    samples = rs.GetInteger("Number of points", 10, 1)
    if samples is None: return
    rs.EnableRedraw(False)
    rs.AddSphere([0,0,0], 1)
    rs.AddPoints(GetPointsEquiAngularlyDistancedOnSphere(samples))
    rs.EnableRedraw(True)    

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SamplePointsOnSphere()