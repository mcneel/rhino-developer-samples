################################################################################
# SampleRebuildSrfDeviation.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import System
import rhinoscriptsyntax as rs

# Description:
#   Returns max deviation of a rebuilt surface
# Parameters:
#   original - A Brep face whose underlying surface was rebuilt
#   rebuild - The rebuilt surface
def SampleRebuildSrfDeviation(original, rebuilt):
    o_domain = []
    r_domain = []
    for i in range(2):
        o_domain.append(original.Domain(i))
        r_domain.append(rebuilt.Domain(i))
    
    knot = []
    for i in range(2):
        k = original.GetSpanVector(i)
        knot.append(k)
    
    delta = []
    samples = 2
    for j0 in range(knot[0].Count - 1):
        u_interval = Rhino.Geometry.Interval(knot[0][j0], knot[0][j0 + 1])
        for j1 in range(knot[1].Count - 1):
            v_interval = Rhino.Geometry.Interval(knot[1][j1], knot[1][j1 + 1])
            for i0 in range(samples + 1):
                for i1 in range(samples + 1):
                    if i0 == 0 and j0 > 0: continue
                    if i1 == 0 and j1 > 0: continue
                    u = u_interval.ParameterAt(i0 / samples)
                    v = v_interval.ParameterAt(i1 / samples)
                    if original.IsPointOnFace(u, v) != Rhino.Geometry.PointFaceRelation.Exterior:
                        po = original.PointAt(u, v)
                        uu = r_domain[0].ParameterAt(o_domain[0].NormalizedParameterAt(u))
                        vv = r_domain[1].ParameterAt(o_domain[1].NormalizedParameterAt(v))
                        rc, ru, rv = rebuilt.LocalClosestPoint(po, uu, vv)
                        if rc:
                            pr = rebuilt.PointAt(ru, rv)
                            delta.append(Rhino.Geometry.Line(po, pr))
    
    max_deviation = 0.0
    for line in delta:
        length = line.Length
        if length > max_deviation:
            max_deviation = length
    
    return max_deviation

# Tests the above function
def TestSampleRebuildSrfDeviation():
    brep_id = rs.GetObject('Select surface to rebuild', 8)
    if brep_id:
        brep = rs.coercebrep(brep_id)
        face = brep.Faces[0]
        srf = brep.Surfaces[face.SurfaceIndex]
        nurb = srf.Rebuild(3, 3, 8, 8)
        print 'Maximum deviation =', SampleRebuildSrfDeviation(face, nurb)

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    TestSampleRebuildSrfDeviation()