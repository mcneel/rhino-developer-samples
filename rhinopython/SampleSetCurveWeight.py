################################################################################
# SampleSetCurveWeight.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import clr
import Rhino
import System
import rhinoscriptsyntax as rs
import scriptcontext as sc

################################################################################
# Set the weight of a curve control point like the Weight command.
# Keeping this version so I have reference to a fancy Overloads example.
################################################################################
def SetCurveWeightEx(nc, i, w):
    nc.Points.MakeRational()
    # There are a couple of versions of NurbsCurvePointList.GetPoint.
    # This code helps call the correct one.
    tref = clr.GetClrType(Rhino.Geometry.Point3d).MakeByRefType() 
    rc, p = nc.Points.GetPoint.Overloads[System.Int32, tref](i)
    if w != 1.0 and Rhino.RhinoMath.IsValidDouble(w):
        nc.Points.SetPoint(i, p, w)
    else:
        nc.Points.SetPoint(i, p, 1.0)

################################################################################
# Set the weight of a curve control point like the Weight command.
################################################################################
def SetCurveWeight(nc, i, w):
    nc.Points.MakeRational()
    p = nc.Points[i].Location
    if w != 1.0 and Rhino.RhinoMath.IsValidDouble(w):
        nc.Points.SetPoint(i, p, w)
    else:
        nc.Points.SetPoint(i, p, 1.0)

################################################################################
# Main function
################################################################################
def SampleSetCurveWeight():
    crv_id = rs.GetObject(preselect=True)
    crv = rs.coercecurve(crv_id)
    
    nc = crv.ToNurbsCurve()
    SetCurveWeight(nc, 2, 0.1)
    
    sc.doc.Objects.Replace(crv_id, nc)
    sc.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleSetCurveWeight()