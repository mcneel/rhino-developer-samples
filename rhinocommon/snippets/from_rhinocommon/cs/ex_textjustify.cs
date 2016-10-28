using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace examples_cs
{
  public class TextJustifyCommand : Command
  {
    public override string EnglishName { get { return "csTextJustify"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var text_entity = new TextEntity
      {
        Plane = Plane.WorldXY,
        Text = "Hello Rhino!",
        Justification = TextJustification.MiddleCenter,
        FontIndex = doc.Fonts.FindOrCreate("Arial", false, false)
      };

      doc.Objects.AddText(text_entity);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}