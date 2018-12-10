import Rhino
import scriptcontext

def ExplodeHatch():
    filter = Rhino.DocObjects.ObjectType.Hatch
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select hatch to explode", False, filter)
    if rc != Rhino.Commands.Result.Success: return

    hatch = objref.Geometry()
    if not hatch: return

    hatch_geom = hatch.Explode()
    if hatch_geom:
        for geom in hatch_geom:
            if geom.ObjectType == Rhino.DocObjects.ObjectType.Point:
                scriptcontext.doc.Objects.AddPoint(geom)
            elif geom.ObjectType == Rhino.DocObjects.ObjectType.Curve:
                scriptcontext.doc.Objects.AddCurve(geom)
            elif geom.ObjectType == Rhino.DocObjects.ObjectType.Brep:
                scriptcontext.doc.Objects.AddBrep(geom)
        scriptcontext.doc.Views.Redraw()

if __name__=="__main__":
    ExplodeHatch()
