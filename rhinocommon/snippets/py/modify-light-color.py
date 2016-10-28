from Rhino import *
from Rhino.DocObjects import *
from Rhino.Input import *
from Rhino.UI import *
from Rhino.Commands import Result
from scriptcontext import doc

def RunCommand():
  rc, obj_ref = RhinoGet.GetOneObject(
    "Select light to change color",
    True,
    ObjectType.Light)
  if rc != Result.Success:
    return rc
  light = obj_ref.Light()
  if light == None:
    return Result.Failure

  b, color = Dialogs.ShowColorDialog(light.Diffuse)
  if b:
    light.Diffuse = color

  doc.Lights.Modify(obj_ref.ObjectId, light)
  return Result.Success

if __name__ == "__main__":
  RunCommand()
