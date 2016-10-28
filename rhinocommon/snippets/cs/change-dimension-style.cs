partial class Examples
{
  public static Result ChangeDimensionStyle(RhinoDoc doc)
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
