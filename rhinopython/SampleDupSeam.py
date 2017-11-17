################################################################################
# SampleDupSeam.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext

# Duplicates the seam of a closed Brep
def SampleDupSeam():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select edges to duplicate")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.SeamEdge
    go.GetMultiple(1,0)
    if (go.CommandResult() == Rhino.Commands.Result.Success):
        for i in range(0, go.ObjectCount):
            ref = go.Object(i)
            edge = ref.Edge()
            crv = edge.DuplicateCurve()
            scriptcontext.doc.Objects.AddCurve(crv)

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleDupSeam() # Call the function defined above