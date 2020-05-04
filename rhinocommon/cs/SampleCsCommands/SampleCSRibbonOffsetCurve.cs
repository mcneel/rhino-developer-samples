using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCSRibbonOffsetCurve : Command
  {
    static SampleCSRibbonOffsetCurve _instance;
    public SampleCSRibbonOffsetCurve()
    {
      _instance = this;
    }

    ///<summary>The only instance of the SampleCSRibbonOffsetCurve command.</summary>
    public static SampleCSRibbonOffsetCurve Instance
    {
      get { return _instance; }
    }

    public override string EnglishName
    {
      get { return "SampleCSRibbonOffsetCurve"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      //Get a closed curve to offset
      var go = new GetObject
      {
        GeometryFilter = ObjectType.Curve,
        GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve
      };
      go.SetCommandPrompt("Select curve to offset");
      go.Get();

      //The user did not select a curve
      if (go.Result() != GetResult.Object)
        return Result.Cancel;

      //Get the curve from the GetObject
      var crv = go.Object(0).Curve();
      if (crv == null)
        return Result.Cancel;

      //Get the side of the curve to offset towards.
      var gp = new GetPoint();
      gp.SetCommandPrompt("Select curve side to offset");
      gp.Get();

      //The user did not select a point
      if (gp.Result() != GetResult.Point)
        return Result.Cancel;
      //The selected Point
      var pt = gp.Point();

      //Try to get the plane from the curve
      crv.TryGetPlane(out Plane ribbon_plane);
      if (!ribbon_plane.IsValid)
        ribbon_plane = doc.Views.ActiveView.ActiveViewport.ConstructionPlane();

      //Set the ribbon plane vector
      var plane_vector = ribbon_plane.Normal;

      //Create the ribbon offset, its cross sections, and its ruled surfaces
      var ribbon_curve_output = crv.RibbonOffset(1, .1, pt, plane_vector, doc.ModelAbsoluteTolerance,out Curve[] cross_sections, out Surface[] ruled_surfaces);

      //Add the ribbon curve to the document
      if (ribbon_curve_output != null)
      {
        doc.Objects.AddCurve(ribbon_curve_output);
      }

      //Add any cross section curves to the document
      if (cross_sections != null)
      {
        foreach (var curve in cross_sections)
        {
          doc.Objects.AddCurve(curve);
        }
      }

      //Add any ruled surfaces to the document
      if (ruled_surfaces != null)
      {
        foreach (var surface in ruled_surfaces)
        {
          doc.Objects.AddSurface(surface);
        }
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}