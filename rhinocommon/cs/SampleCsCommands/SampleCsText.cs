using System;
using System.Drawing;
using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsText : Command
  {
    private TextJustification m_justification = TextJustification.None;

    public override string EnglishName
    {
      get { return "SampleCsText"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gp = new GetPoint();
      gp.SetCommandPrompt("Start point");
      gp.AddOptionEnumList("Justification", m_justification);
      gp.ConstrainToConstructionPlane(false);
      for (;;)
      {
        var res = gp.Get();
        if (res == GetResult.Option)
        {
          var option = gp.Option();
          if (null != option)
          {
            var list = Enum.GetValues(typeof(TextJustification)).Cast<TextJustification>().ToList();
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

      var point = gp.Point();

      var plane = gp.View().ActiveViewport.ConstructionPlane();
      plane.Origin = point;

      var text = new TextEntity
      {
        Plane = plane,
        Justification = m_justification
      };
      text.PlainText = text.Justification.ToString();

      var attr = new ObjectAttributes
      {
        ColorSource = ObjectColorSource.ColorFromObject,
        ObjectColor = Color.FromArgb(0, 0, 255)
      };

      var object_id = doc.Objects.AddText(text, attr);
      RhinoApp.WriteLine("{0}", object_id.ToString());
      
      doc.Views.Redraw();

      return Result.Success;
    }

  }
}
