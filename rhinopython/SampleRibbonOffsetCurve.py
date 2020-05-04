################################################################################
# SampleRibbonOffsetCurve.py
# Copyright (c) 2020 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import clr
import Rhino
import scriptcontext
import System.Drawing.Color
import System.Collections.Generic.IEnumerable as IEnumerable

def SampleCreateRibbon():
    
    go = Rhino.Input.Custom.GetObject()
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve
    go.SetCommandPrompt("Select closed curve to offset")
    go.Get()
    if(go.CommandResult() != Rhino.Commands.Result.Success):
        return go.CommandResult()

    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Select curve side to offset")
    gp.Get()
    if(gp.CommandResult() != Rhino.Commands.Result.Success):
        return gp.CommandResult()

    crv = go.Object(0).Curve()
    pt = gp.Point()
    plane = scriptcontext.doc.ActiveDoc.Views.ActiveView.ActiveViewport.ConstructionPlane()
    direction = plane.Normal
    tol = scriptcontext.doc.ModelAbsoluteTolerance

    rib = crv.RibbonOffset(2.0,1.0,pt,direction,tol)
    scriptcontext.doc.ActiveDoc.Objects.Add(rib)

    return Rhino.Commands.Result.Success

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if( __name__ == '__main__' ):
    SampleCreateRibbon()