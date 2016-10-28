from Rhino.Commands import *
import rhinoscriptsyntax as rs
from scriptcontext import doc

def RunCommand():
  view = doc.Views.ActiveView
  if view == None:
    return Result.Failure
 
  view.MainViewport.Name = "Facade"
  return Result.Success

if __name__ == "__main__":
  RunCommand()