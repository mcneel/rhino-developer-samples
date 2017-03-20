using System;
using System.Collections.Generic;
using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("6c645108-0f9d-48c1-8b68-4a0237354693")]
  public class SampleCsText : Command
  {
    private TextJustification m_justification = TextJustification.None;

    public SampleCsText()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsText"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Start point");
      gp.AddOptionEnumList("Justification", m_justification);
      gp.ConstrainToConstructionPlane(false);
      for (;;)
      {
        GetResult res = gp.Get();
        if (res == GetResult.Option)
        {
          CommandLineOption option = gp.Option();
          if (null != option)
          {
            List<TextJustification> list = Enum.GetValues(typeof(TextJustification)).Cast<TextJustification>().ToList();
            m_justification = list[option.CurrentListOptionIndex];
          }
          continue;
        }
        else if (res != GetResult.Point)
        {
          return Result.Cancel;
        }
        break;
      }

      Point3d point = gp.Point();

      Plane plane = gp.View().ActiveViewport.ConstructionPlane();
      plane.Origin = point;

      TextEntity text = new TextEntity();
      text.Plane = plane;
      text.Justification = m_justification;
      text.Text = text.Justification.ToString();

      doc.Objects.AddPoint(point);
      doc.Objects.AddText(text);
      doc.Views.Redraw();

      return Result.Success;
    }

  }
}
