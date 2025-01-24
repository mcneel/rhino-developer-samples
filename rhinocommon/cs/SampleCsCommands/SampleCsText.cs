using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using System;
using System.Drawing;
using System.Linq;

namespace SampleCsCommands
{
  public class SampleCsText : Command
  {
    private TextJustification m_justification = TextJustification.None;

    public override string EnglishName => "SampleCsText";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Start point");
      gp.AddOptionEnumList("Justification", m_justification);
      gp.ConstrainToConstructionPlane(false);
      for (; ; )
      {
        GetResult res = gp.Get();
        if (res == GetResult.Option)
        {
          CommandLineOption option = gp.Option();
          if (null != option)
          {
            System.Collections.Generic.List<TextJustification> list = Enum.GetValues(typeof(TextJustification)).Cast<TextJustification>().ToList();
            m_justification = list[option.CurrentListOptionIndex];
          }
          continue;
        }
        if (res != GetResult.Point)
        {
          return Result.Cancel;
        }
        break;
      }

      Point3d point = gp.Point();

      Plane plane = gp.View().ActiveViewport.ConstructionPlane();
      plane.Origin = point;

      TextEntity text = new TextEntity
      {
        Plane = plane,
        Justification = m_justification
      };
      text.PlainText = text.Justification.ToString();

      ObjectAttributes attr = new ObjectAttributes
      {
        ColorSource = ObjectColorSource.ColorFromObject,
        ObjectColor = Color.FromArgb(0, 0, 255)
      };

      Guid object_id = doc.Objects.AddText(text, attr);
      RhinoApp.WriteLine("{0}", object_id.ToString());

      doc.Views.Redraw();

      return Result.Success;
    }

  }
}
