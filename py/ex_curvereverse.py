import rhinoscriptsyntax as rs
from scriptcontext import *
import Rhino

def ReverseCurves():
    crvs = rs.GetObjects("Select curves to reverse", rs.filter.curve)
    if not crvs: return
    
    for crvid in crvs:
        crv = rs.coercecurve(crvid)
        if not crv: continue
        dup = crv.DuplicateCurve()
        if dup:
            dup.Reverse()
        doc.Objects.Replace(crvid, dup)

if __name__ == "__main__":
    ReverseCurves()