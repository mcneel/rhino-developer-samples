using Rhino;
using Rhino.DocObjects;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace examples_cs
{
  public class ReadDimensionTextCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csReadDimensionText"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select annotation");
      go.GeometryFilter = ObjectType.Annotation;
      go.Get();
      if (go.CommandResult() != Result.Success) 
        return Result.Failure;
      var annotation = go.Object(0).Object() as AnnotationObjectBase;
      if (annotation == null)
        return Result.Failure;

      RhinoApp.WriteLine("Annotation text = {0}", annotation.DisplayText);

      return Result.Success;
    }
  }
}