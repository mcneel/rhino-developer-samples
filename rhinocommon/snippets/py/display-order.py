from System.Collections.Generic import *
from System.Drawing import *
from Rhino import *
from Rhino.Commands import *
from Rhino.Display import *
from Rhino.Geometry import *
from Rhino.Input import *
from Rhino.DocObjects import *
from scriptcontext import doc

m_line_objects = []

def RunCommand():
    # make lines thick so draw order can be easily seen
    wfdm = [dm for dm in DisplayModeDescription.GetDisplayModes() if dm.EnglishName == "Wireframe"][0]
    original_thikcness = wfdm.DisplayAttributes.CurveThickness
    wfdm.DisplayAttributes.CurveThickness = 10
    DisplayModeDescription.UpdateDisplayMode(wfdm)

    AddLine(Point3d.Origin, Point3d(10,10,0), Color.Red, doc)
    AddLine(Point3d(10,0,0), Point3d(0,10,0), Color.Blue, doc)
    AddLine(Point3d(8,0,0), Point3d(8,10,0), Color.Green, doc)
    AddLine(Point3d(0,3,0), Point3d(10,3,0), Color.Yellow, doc)
    doc.Views.Redraw()
    Pause("draw order: 1st line drawn in front, last line drawn in the back.  Any key to continue ...")

    #all objects have a DisplayOrder of 0 by default so changing it to 1 moves it to the front.  Here we move the 2nd line (blue) to the front
    m_line_objects[1].Attributes.DisplayOrder = 1
    m_line_objects[1].CommitChanges()
    doc.Views.Redraw()
    Pause("Second (blue) line now in front.  Any key to continue ...")

    for i in range(1, m_line_objects.Count):
        m_line_objects[i].Attributes.DisplayOrder = i
        m_line_objects[i].CommitChanges()

    doc.Views.Redraw()
    Pause("Reverse order of original lines, i.e., Yellow 1st and Red last.  Any key to continue ...")

    # restore original line thickness
    wfdm.DisplayAttributes.CurveThickness = original_thikcness
    DisplayModeDescription.UpdateDisplayMode(wfdm)

    doc.Views.Redraw()
    return Result.Success

def Pause(msg):
    rc, obj_ref = RhinoGet.GetOneObject(msg, True, ObjectType.AnyObject)

def AddLine(from_pt, to_pt, color, doc):
    guid = doc.Objects.AddLine(from_pt, to_pt)
    obj = doc.Objects.Find(guid)
    m_line_objects.Add(obj)
    obj.Attributes.ObjectColor = color
    obj.Attributes.ColorSource = ObjectColorSource.ColorFromObject
    obj.CommitChanges()

if __name__ == "__main__":
    RunCommand()
