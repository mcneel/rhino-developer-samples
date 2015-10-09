using Rhino;
using Rhino.DocObjects;
using Rhino.Commands;
using Rhino.Geometry;

namespace examples_cs
{
  public class ChangeDimensionStyleCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csChangeDimensionStyle"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      foreach (var rhino_object in doc.Objects.GetObjectList(ObjectType.Annotation))
      {
        var annotation_object = rhino_object as AnnotationObjectBase;
        if (annotation_object == null) continue;

        var annotation = annotation_object.Geometry as AnnotationBase;
        if (annotation == null) continue;

        if (annotation.Index == doc.DimStyles.CurrentDimensionStyleIndex) continue;

        annotation.Index = doc.DimStyles.CurrentDimensionStyleIndex;
        annotation_object.CommitChanges();
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}