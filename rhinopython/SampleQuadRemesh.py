################################################################################
# SampleQuadRemesh.py
# Copyright (c) 2020 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext


def SampleQuadRemesh():
    
    go = Rhino.Input.Custom.GetObject()
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Mesh
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve
    go.SetCommandPrompt("Select Mesh to QuadRemesh")
    go.Get()
    if(go.CommandResult() != Rhino.Commands.Result.Success):
        return go.CommandResult()

    mesh = go.Object(0).Mesh()

    qr_params = Rhino.Geometry.QuadRemeshParameters()
    qr_params.AdaptiveQuadCount=True
    qr_params.TargetQuadCount = 2000
    qr_params.AdaptiveSize = 50
    qr_params.DetectHardEdges=True
    qr_params.SymmetryAxis = Rhino.Geometry.QuadRemeshSymmetryAxis.X | Rhino.Geometry.QuadRemeshSymmetryAxis.Y | Rhino.Geometry.QuadRemeshSymmetryAxis.Z
    
    
    remeshed = mesh.QuadRemesh(qr_params)
    
    
    scriptcontext.doc.ActiveDoc.Objects.Add(remeshed)
    scriptcontext.doc.ActiveDoc.Views.Redraw

    return Rhino.Commands.Result.Success

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if( __name__ == '__main__' ):
    SampleQuadRemesh()