using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Display;
using Rhino.Input.Custom;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsOrientPerpendicularToCurve : Command
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsOrientPerpendicularToCurve()
    {
    }

    /// <summary>
    /// Command name
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsOrientPerpendicularToCurve"; }
    }

    /// <summary>
    /// Runs the command
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select objects to orient
      var go = new GetObject();
      go.SetCommandPrompt("Select objects to orient");
      go.SubObjectSelect = false;
      go.GroupSelect = true;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // Point to orient from
      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to orient from");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      // Define source plane
      var view = gp.View();
      if (view == null)
      {
        view = doc.Views.ActiveView;
        if (view == null)
          return Result.Failure;
      }

      var plane = view.ActiveViewport.ConstructionPlane();
      plane.Origin = gp.Point();

      // Curve to orient on
      var gc = new GetObject();
      gc.SetCommandPrompt("Curve to orient on");
      gc.GeometryFilter = ObjectType.Curve;
      gc.EnablePreSelect(false, true);
      gc.DeselectAllBeforePostSelect = false;
      gc.Get();
      if (gc.CommandResult() != Result.Success)
        return gc.CommandResult();

      var objref = gc.Object(0);
      var obj = objref.Object();
      var curve = objref.Curve();
      if (obj == null || curve == null)
        return Result.Failure;

      // Unselect curve
      obj.Select(false);

      // Point on surface to orient to
      var gx = new GetOrientPerpendicularPoint(curve, plane, go.Object(0).Object());
      gx.SetCommandPrompt("New base point on curve");
      gx.Get();
      if (gx.CommandResult() != Result.Success)
        return gx.CommandResult();

      // One final calculation
      var xform = new Transform(1);
      if (gx.CalculateTransform(gx.View().ActiveViewport, gx.Point(), ref xform))
      {
        doc.Objects.Transform(go.Object(0).Object(), xform, true);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }

  /// <summary>
  /// GetOrientPerpendicularPoint class
  /// </summary>
  class GetOrientPerpendicularPoint : GetPoint
  {
    private readonly Curve m_curve;
    private readonly Plane m_plane;
    private readonly RhinoObject m_obj;
    private Transform m_xform;
    private bool m_draw;

    /// <summary>
    /// Constructor
    /// </summary>
    public GetOrientPerpendicularPoint(Curve curve, Plane plane, RhinoObject obj)
    {
      m_curve = curve;
      m_plane = plane;
      m_obj = obj;

      m_xform = new Transform(1);
      m_draw = false;

      Constrain(m_curve, false);
      MouseMove += OnMouseMove;
      DynamicDraw += OnDynamicDraw;
    }

    /// <summary>
    /// Calculate the transformation
    /// </summary>
    public bool CalculateTransform(RhinoViewport vp, Point3d pt, ref Transform xform)
    {
      bool rc = false;
      if (null != m_curve)
      {
        double t;
        if (m_curve.ClosestPoint(pt, out t))
        {
          Plane frame;
          if (m_curve.PerpendicularFrameAt(t, out frame))
          {
            xform = Transform.PlaneToPlane(m_plane, frame);
            rc = xform.IsValid;
          }
        }
      }
      return rc;
    }

    /// <summary>
    /// MouseMove event handler
    /// </summary>
    void OnMouseMove(object sender, GetPointMouseEventArgs e)
    {
      m_draw = CalculateTransform(e.Viewport, e.Point, ref m_xform);
    }

    /// <summary>
    /// DynamicDraw event handler
    /// </summary>
    void OnDynamicDraw(object sender, GetPointDrawEventArgs e)
    {
      if (m_draw)
      {
        if (null != m_obj)
          e.Display.DrawObject(m_obj, m_xform);
      }
    }
  }
}
