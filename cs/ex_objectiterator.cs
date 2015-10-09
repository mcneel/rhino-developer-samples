using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace examples_cs
{
  public class ObjectEnumeratorCommand : Command
  {
    public override string EnglishName
    {
      get { return "csObjectEnumerator"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var object_enumerator_settings = new ObjectEnumeratorSettings();
      object_enumerator_settings.IncludeLights = true;
      object_enumerator_settings.IncludeGrips = false;
      var rhino_objects = doc.Objects.GetObjectList(object_enumerator_settings);

      int count = 0;
      foreach (var rhino_object in rhino_objects)
      {
        if (rhino_object.IsSelectable() && rhino_object.IsSelected(false) == 0)
        {
          rhino_object.Select(true);
          count++;
        }
      }
      if (count > 0)
      {
        doc.Views.Redraw();
        RhinoApp.WriteLine("{0} object{1} selected", count,
          count == 1 ? "" : "s");
      }
      return Result.Success;
    }
  }
}