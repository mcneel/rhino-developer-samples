from Rhino import *
from Rhino.Commands import *
from Rhino.Input.Custom import *

def RunCommand():
  go = GetObject()
  go.SetCommandPrompt("Select objects")
  go.EnablePreSelect(True, True)
  go.EnablePostSelect(True)
  go.GetMultiple(0, 0)
  if go.CommandResult() != Result.Success:
    return go.CommandResult()

  selected_objects = []
  for obj in go.Objects():
    selected_objects.Add(obj)

  if go.ObjectsWerePreselected:
    go.EnablePreSelect(False, True)
    go.DeselectAllBeforePostSelect = False
    go.EnableUnselectObjectsOnExit(False)
    go.GetMultiple(0, 0)
    if go.CommandResult() == Result.Success:
      for obj in go.Objects():
        selected_objects.Add(obj)
        # The normal behavior of commands is that when they finish,
        # objects that were pre-selected remain selected and objects
        # that were post-selected will not be selected. Because we
        # potentially could have both, we'll try to do something
        # consistent and make sure post-selected objects also stay selected
        obj.Object().Select(True)
  return Result.Success if selected_objects.Count > 0 else Result.Nothing

if __name__ == "__main__":
  RunCommand()
