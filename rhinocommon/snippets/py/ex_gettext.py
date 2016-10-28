from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Input.Custom import *
import rhinoscriptsyntax as rs

def RunCommand():
  go = GetObject()
  go.SetCommandPrompt("Select annotation")
  go.GeometryFilter = ObjectType.Annotation
  go.Get()
  if go.CommandResult() <> Result.Success:
    return Result.Failure
  annotation = go.Object(0).Object()
  if annotation == None or not isinstance(annotation, AnnotationObjectBase):
    return Result.Failure

  print "Annotation text = {0}".format(annotation.DisplayText)

  return Result.Success

if __name__ == "__main__":
  RunCommand()