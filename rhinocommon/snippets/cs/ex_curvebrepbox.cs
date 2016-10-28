using Rhino;
using Rhino.Geometry;
using Rhino.Commands;
using Rhino.Input;
using Rhino.DocObjects;

namespace examples_cs
{
  public class BrepFromCurveBBoxCommand : Command
  {
    public override string EnglishName { get { return "csBrepFromCurveBBox"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.ObjRef objref;
      var rc = RhinoGet.GetOneObject("Select Curve", false, ObjectType.Curve, out objref);
      if( rc != Result.Success )
        return rc;
      var curve = objref.Curve();

      var view = doc.Views.ActiveView;
      var plane = view.ActiveViewport.ConstructionPlane();
      // Create a construction plane aligned bounding box
      var bbox = curve.GetBoundingBox(plane);

      if (bbox.IsDegenerate(doc.ModelAbsoluteTolerance) > 0) {
        RhinoApp.WriteLine("the curve's bounding box is degenerate (flat) in at least one direction so a box cannot be created.");
        return Result.Failure;
      }
      var brep = Brep.CreateFromBox(bbox);
      doc.Objects.AddBrep(brep);
      doc.Views.Redraw();
      return Result.Success;
    }
  }
}