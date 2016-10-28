import Rhino
import scriptcontext
import System.Guid

def DetermineObjectLayer():
    rc, obref = Rhino.Input.RhinoGet.GetOneObject("Select object", True, Rhino.DocObjects.ObjectType.AnyObject)
    if rc!=Rhino.Commands.Result.Success: return rc
    rhobj = obref.Object()
    if rhobj is None: return Rhino.Commands.Result.Failure
    index = rhobj.Attributes.LayerIndex
    name = scriptcontext.doc.Layers[index].Name
    print "The selected object's layer is '", name, "'"
    return Rhino.Commands.Result.Success

if __name__ == "__main__":
    DetermineObjectLayer()
