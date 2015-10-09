from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from scriptcontext import doc

def RunCommand():
  object_enumerator_settings = ObjectEnumeratorSettings()
  object_enumerator_settings.IncludeLights = True
  object_enumerator_settings.IncludeGrips = False
  rhino_objects = doc.Objects.GetObjectList(object_enumerator_settings)

  count = 0
  for rhino_object in rhino_objects:
    if rhino_object.IsSelectable() and rhino_object.IsSelected(False) == 0:
      rhino_object.Select(True)
      count += 1;

  if count > 0:
    doc.Views.Redraw()
    RhinoApp.WriteLine("{0} object{1} selected", count,
      "" if count == 1 else "s")

  return Result.Success

if __name__ == "__main__":
  RunCommand()