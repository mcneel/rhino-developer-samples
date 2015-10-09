using Rhino;
using Rhino.Input;
using Rhino.Input.Custom;
using Rhino.DocObjects;
using Rhino.Commands;

namespace examples_cs
{
  public class CurveClosestPointCommand : Command
  {
    public override string EnglishName { get { return "csFindCurveParameterAtPoint"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.ObjRef objref;
      var rc = RhinoGet.GetOneObject("Select curve", true, ObjectType.Curve,out objref);
      if(rc!= Result.Success)
        return rc;
      var curve = objref.Curve();
      if( curve==null )
        return Result.Failure;

      var gp = new GetPoint();
      gp.SetCommandPrompt("Pick a location on the curve");
      gp.Constrain(curve, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var point = gp.Point();
      double closest_point_param;
      if (curve.ClosestPoint(point, out closest_point_param))
      {
        RhinoApp.WriteLine("point: ({0}), parameter: {1}", point, closest_point_param);
        doc.Objects.AddPoint(point);
        doc.Views.Redraw();
      }
      return Result.Success;
    }
  }
}
