from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.Input.Custom import *
from scriptcontext import doc

def RunCommand():
  rc, obj_refs = RhinoGet.GetMultipleObjects("Select hatches to replace", False, ObjectType.Hatch)
  if rc != Result.Success or obj_refs == None:
    return rc

  gs = GetString()
  gs.SetCommandPrompt("Name of replacement hatch pattern")
  gs.AcceptNothing(False)
  gs.Get()
  if gs.CommandResult() != Result.Success:
    return gs.CommandResult()
  hatch_name = gs.StringResult()

  pattern_index = doc.HatchPatterns.Find(hatch_name, True)

  if pattern_index < 0:
    RhinoApp.WriteLine("The hatch pattern \"{0}\" not found  in the document.", hatch_name)
    return Result.Nothing

  for obj_ref in obj_refs:
    hatch_object = obj_ref.Object()
    if hatch_object.HatchGeometry.PatternIndex != pattern_index:
      hatch_object.HatchGeometry.PatternIndex = pattern_index
      hatch_object.CommitChanges()

  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()
