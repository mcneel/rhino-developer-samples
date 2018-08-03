################################################################################
# SampleEditPoints.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import rhinoscriptsyntax as rs

# Demonstrates how to get a curve's edit points
def SampleEditPoints():
    
    obj_id = rs.GetObject('Select curve', rs.filter.curve)
    if not obj_id: return
    
    crv = rs.coercecurve(obj_id)
    if not crv: return
    
    nc = crv.ToNurbsCurve()
    
    # NurbsCurve.GrevilleParameters always returns all parameters, even for
    # perodic curves. Otherwise, it would not be possible to determine
    # which subset of the full list of Greville abcissae was returned.
    t_list = nc.GrevilleParameters()
    
    # Filter out what we don't want so it looks like what we'd see if
    # we ran the EditPtOn command
    if t_list:
        t = 0
        t_count = t_list.Count
        if nc.IsPeriodic:
            t_count -= nc.Order - 1
            rc = 0
            while True:
                if rc < nc.Order - 1 and t_list[t] < nc.Knots[nc.Order-2]:
                    rc += 1
                    t += 1
                    continue
                break
        elif nc.IsClosed:
            t_count -= 1
            
        for n in range(t, t + t_count):
            rs.AddPoint(nc.PointAt(t_list[n]))

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleEditPoints()