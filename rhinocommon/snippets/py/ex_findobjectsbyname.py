import Rhino
import scriptcontext
import System.Guid

def FindObjectsByName():
    name = "abc"
    settings = Rhino.DocObjects.ObjectEnumeratorSettings()
    settings.NameFilter = name
    ids = [rhobj.Id for rhobj in scriptcontext.doc.Objects.GetObjectList(settings)]
    if not ids:
        print "No objects with the name", name
        return Rhino.Commands.Result.Failure
    else:
        print "Found", len(ids), "objects"
        for id in ids: print "  ", id
    return Rhino.Commands.Result.Success

if __name__ == "__main__":
    FindObjectsByName()
