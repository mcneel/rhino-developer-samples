################################################################################
# SampleCurveDivideByCount.py -- June 2016
# Copyright (c) Robert McNeel & Associates
################################################################################
import Rhino
import scriptcontext as sc
import clr
import System

def SampleCurveDivideByCount():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select curve")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.Get()
    if go.CommandResult() == Rhino.Commands.Result.Success:
        curve = go.Object(0).Curve()
        if curve:
            #parameters = curve.DivideByCount(5, True)
            points = clr.StrongBox[System.Array[Rhino.Geometry.Point3d]]()
            parameters = curve.DivideByCount(5, True, points)
            points = list(points.Value)
            for pt in points:
                sc.doc.Objects.AddPoint(pt)
            sc.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleCurveDivideByCount() # Call the function defined above