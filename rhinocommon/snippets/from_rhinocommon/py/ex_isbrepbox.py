import Rhino

def IsBrepBox(brep):
    zero_tolerance = 1.0e-6 #or whatever
    rc = brep.IsSolid
    if rc: rc = brep.Faces.Count == 6

    N = []
    for i in range(6):
        if not rc: break
        rc, plane = brep.Faces[i].TryGetPlane(zero_tolerance)
        if rc:
            v = plane.ZAxis
            v.Unitize()
            N.append(v)
 
    for i in range(6):
        count = 0
        for j in range(6):
            if not rc: break
            dot = abs(N[i] * N[j])
            if dot<=zero_tolerance: continue
            if abs(dot-1.0)<=zero_tolerance:
              count += 1
            else:
              rc = False

    if rc:
        if 2!=count: rc = False
    return rc


if __name__=="__main__":
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select Brep", True, Rhino.DocObjects.ObjectType.Brep)
    if rc==Rhino.Commands.Result.Success:
        brep = objref.Brep()
        if brep:
            if IsBrepBox(brep): print "Yes it is a box"
            else: print "No it is not a box"
