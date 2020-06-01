using System;
using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsSilhouetteDraftCurve : Command
  {
    static SampleCsSilhouetteDraftCurve _instance;
    public SampleCsSilhouetteDraftCurve()
    {
      _instance = this;
    }

    ///<summary>The only instance of the SampleCSShilhouetteDraftCurve command.</summary>
    public static SampleCsSilhouetteDraftCurve Instance
    {
      get { return _instance; }
    }

    public override string EnglishName
    {
      get { return "SampleCsSilhouetteDraftCurve"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {

      //Select a brep or mesh to extract some curves from
      var go = new GetObject() {GeometryFilter = ObjectType.Brep | ObjectType.Mesh};
      go.SetCommandPrompt("Select object for draft curve extraction");
      go.Get();
      if (go.Result() != GetResult.Object)
        return Result.Cancel;

      //The selected geometry base
      var geometry = go.Object(0).Geometry();


      //Min & Max angle must be specified in Radians
      double min_angle = RhinoMath.ToRadians(-5.00);
      double max_angle = RhinoMath.ToRadians(5.00);

      //Set the vector direction of the draft computation
      Vector3d direction = Vector3d.ZAxis * -1;

      //Set the tolerances
      double tolerance = doc.ModelAbsoluteTolerance;
      double angle_tolerance = doc.ModelAngleToleranceRadians;

      //Compute the minimum draft angle curves
      var min_draft_silhouettes = Silhouette.ComputeDraftCurve(geometry, min_angle, direction, tolerance,angle_tolerance);

      //Compute the maximum draft angle curves
      var max_draft_silhouettes = Silhouette.ComputeDraftCurve(geometry, max_angle, direction, tolerance, angle_tolerance);
      

      //Add all of the minimum draft angle silhouette curves to the doc
      if (min_draft_silhouettes != null)
      {
        var oa = new ObjectAttributes(){ObjectColor = Color.Red,ColorSource = ObjectColorSource.ColorFromObject};
        foreach (var silhouette in min_draft_silhouettes)
        {
          var crv = silhouette.Curve;
          doc.Objects.AddCurve(crv,oa);
        }
      }


      //Add all of the maximum draft angle silhouette curves to the doc
      if (max_draft_silhouettes != null)
      {
        var oa = new ObjectAttributes(){ObjectColor = Color.Blue, ColorSource = ObjectColorSource.ColorFromObject };
        foreach (var silhouette in max_draft_silhouettes)
        {
          var crv = silhouette.Curve;
          doc.Objects.AddCurve(crv,oa);
        }
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}