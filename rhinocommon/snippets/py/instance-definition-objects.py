import Rhino
import scriptcontext

def InstanceDefinitionObjects():
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select instance", False, Rhino.DocObjects.ObjectType.InstanceReference)
    if rc != Rhino.Commands.Result.Success: return

    iref = objref.Object()
    if iref:
        idef = iref.InstanceDefinition
        if idef:
            rhino_objects = idef.GetObjects()
            for i, rhobj in enumerate(rhino_objects):
                print "Object", i, "=", rhobj.Id

if __name__=="__main__":
    InstanceDefinitionObjects()
