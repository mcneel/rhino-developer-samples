################################################################################
# SampleBlockLeader.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import scriptcontext
import rhinoscriptsyntax as rs
import Rhino

# Creates a block leader.
def SampleBlockLeader():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select block instance to annotate")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.InstanceReference
    go.EnablePreSelect(False, True)
    go.InactiveDetailPickEnabled = True
    rc = go.Get()
    if rc == Rhino.Input.GetResult.Object:
        prompt0 = "First point of leader"
        prompt1 = "Next point of leader. Press Enter when done"
        points = rs.GetPoints(True, True, prompt0, prompt1)
        if points is not None:
            objref = go.Object(0)
            obj = objref.Object()
            name = rs.BlockInstanceName(obj.Id)
            rs.AddLeader(points, None, name)

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleBlockLeader()    