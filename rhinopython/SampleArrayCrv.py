################################################################################
# SampleArrayCrv.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import rhinoscriptsyntax as rs
import scriptcontext as sc

# Array selected objects along a curve
def SampleArrayCrv():
    
    obj_ids = rs.GetObjects("Select objects to array")
    if not obj_ids: return
    
    base_pt = rs.GetPoint("Base point")
    if not base_pt: return
    
    plane = rs.ViewCPlane()
    plane.Origin = base_pt
    
    crv_id = rs.GetObject("Select path curve")
    if not crv_id: return
    
    count = rs.GetInteger("Number of items", 2, 2)
    if not count: return
    
    if rs.IsCurveClosed(crv_id): count -= 1
    
    crv_t = rs.DivideCurve(crv_id, count, False, False)
    for t in crv_t:
        frame = rs.CurveFrame(crv_id, t)
        xform = rs.XformRotation1(plane, frame)
        rs.TransformObjects(obj_ids, xform, True)

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.   
if __name__ == "__main__":
    SampleArrayCrv()
