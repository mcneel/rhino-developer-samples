from Rhino import *
from Rhino.Commands import *
from Rhino.DocObjects import *
from Rhino.Input import *
from scriptcontext import doc

def RunCommand():
  rs, obj_ref = RhinoGet.GetOneObject(
    "Select object", False, ObjectType.AnyObject)
  if rs <> Result.Success:
    return rs
  rhino_object = obj_ref.Object()
  if rhino_object == None:
    return Result.Failure

  rhino_object_groups = [
    group for group in rhino_object.Attributes.GetGroupList()]

  selectable_objects= [
    obj for obj in doc.Objects.GetObjectList(ObjectType.AnyObject)
    if obj.IsSelectable(True, False, False, False)]

  for selectable_object in selectable_objects:
    for group in selectable_object.Attributes.GetGroupList():
      if rhino_object_groups.Contains(group):
          selectable_object.Select(True)
          continue

  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()