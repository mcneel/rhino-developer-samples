from System import *
from Rhino import *
from Rhino.Commands import *
from Rhino.DocObjects import *
from Rhino.Input import *
from scriptcontext import doc

def RunCommand():
  
  rc, obj_ref = RhinoGet.GetOneObject("Select object to duplicate", False, ObjectType.AnyObject)
  if rc <> Result.Success:
    return rc
  rhino_object = obj_ref.Object()

  geometry_base = rhino_object.DuplicateGeometry()
  if geometry_base <> None:
    if doc.Objects.Add(geometry_base) <> Guid.Empty:
      doc.Views.Redraw()

  return Result.Success

if __name__ == "__main__":
  RunCommand()