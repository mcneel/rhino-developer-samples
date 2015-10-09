import Rhino
import scriptcontext

def IsocurveDensity():
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select surface", False, Rhino.DocObjects.ObjectType.Surface)
    if rc!= Rhino.Commands.Result.Success: return

    brep_obj = objref.Object()
    if brep_obj:
        brep_obj.Attributes.WireDensity = 3
        brep_obj.CommitChanges()
        scriptcontext.doc.Views.Redraw()

if __name__=="__main__":
    IsocurveDensity()