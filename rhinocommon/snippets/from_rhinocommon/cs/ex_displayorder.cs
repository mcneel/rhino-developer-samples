using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.DocObjects;

namespace examples_cs
{
  public class DisplayOrderCommand : Command
  {
    public override string EnglishName { get { return "csDisplayOrder"; } }

    private List<RhinoObject> m_line_objects = new List<RhinoObject>(); 

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // make lines thick so draw order can be easily seen
      var dm = DisplayModeDescription.GetDisplayModes().Single(x => x.EnglishName == "Wireframe");
      var original_thikcness = dm.DisplayAttributes.CurveThickness;
      dm.DisplayAttributes.CurveThickness = 10;
      DisplayModeDescription.UpdateDisplayMode(dm);

      AddLine(Point3d.Origin, new Point3d(10,10,0), Color.Red, doc);
      AddLine(new Point3d(10,0,0), new Point3d(0,10,0), Color.Blue, doc);
      AddLine(new Point3d(8,0,0), new Point3d(8,10,0), Color.Green, doc);
      AddLine(new Point3d(0,3,0), new Point3d(10,3,0), Color.Yellow, doc);
      doc.Views.Redraw();
      Pause("draw order: 1st line drawn in front, last line drawn in the back.  Any key to continue ...");

      //all objects have a DisplayOrder of 0 by default so changing it to 1 moves it to the front.  Here we move the 2nd line (blue) to the front
      m_line_objects[1].Attributes.DisplayOrder = 1;
      m_line_objects[1].CommitChanges();
      doc.Views.Redraw();
      Pause("Second (blue) line now in front.  Any key to continue ...");

      for (int i = 0; i < m_line_objects.Count; i++)
      {
        m_line_objects[i].Attributes.DisplayOrder = i;
        m_line_objects[i].CommitChanges();
      }
      doc.Views.Redraw();
      Pause("Reverse order of original lines, i.e., Yellow 1st and Red last.  Any key to continue ...");

      // restore original line thickness
      dm.DisplayAttributes.CurveThickness = original_thikcness;
      DisplayModeDescription.UpdateDisplayMode(dm);

      doc.Views.Redraw();
      return Result.Success;
    }

    private void Pause(string msg)
    {
      ObjRef obj_ref;
      var rc = RhinoGet.GetOneObject(msg, true, ObjectType.AnyObject, out obj_ref);
    }

    private void AddLine(Point3d from, Point3d to, Color color, RhinoDoc doc)
    {
      var guid = doc.Objects.AddLine(from, to);
      var obj = doc.Objects.Find(guid);
      m_line_objects.Add(obj);
      obj.Attributes.ObjectColor = color;
      obj.Attributes.ColorSource = ObjectColorSource.ColorFromObject;
      obj.CommitChanges();
    }
  }
}