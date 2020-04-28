using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Input;
using Rhino.Input.Custom;
using Rhino.Geometry;
using Rhino.UI.Gumball;

namespace SampleCsCommands
{
  public class SampleCsGumballCylinderCommand : Command
  {
    double m_radius;
    double m_height;

    public SampleCsGumballCylinderCommand()
    {
      m_radius = 1.0;
      m_height = 1.0;
    }

    public override string EnglishName => "SampleCsGumballCylinder";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Base of cylinder");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      Point3d center = gp.Point();
      Plane plane = Plane.WorldXY;
      RhinoView view = gp.View();
      if (null != view)
        plane = view.ActiveViewport.ConstructionPlane();
      plane.Origin = center;

      var cylinder = new SampleCsGumballCylinder(plane, m_radius, m_height);

      var radius_go = new GumballObject();
      var height_go = new GumballObject();

      var radius_dc = new GumballDisplayConduit();
      var height_dc = new GumballDisplayConduit();

      var radius_gas = RadiusGumballAppearanceSettings();
      var height_gas = HeightGumballAppearanceSettings();

      while (true)
      {
        radius_go.SetFromPlane(cylinder.RadiusPlane);
        height_go.SetFromPlane(cylinder.HeightPlane);

        radius_dc.SetBaseGumball(radius_go, radius_gas);
        height_dc.SetBaseGumball(height_go, height_gas);

        radius_dc.Enabled = true;
        height_dc.Enabled = true;

        var gx = new SampleCsGumballCylinderGetPoint(cylinder, radius_dc, height_dc);
        gx.SetCommandPrompt("Drag gumball. Press Enter when done");
        gx.AcceptNothing(true);
        gx.MoveGumball();

        radius_dc.Enabled = false;
        height_dc.Enabled = false;

        if (gx.CommandResult() != Result.Success)
          break;

        var res = gx.Result();
        if (res == GetResult.Point)
        {
          var radius = cylinder.Radius;
          var height = cylinder.Height;
          cylinder = new SampleCsGumballCylinder(plane, radius, height);
          continue;
        }
        if (res == GetResult.Nothing)
        {
          m_radius = cylinder.Radius;
          m_height = cylinder.Height;
          cylinder = new SampleCsGumballCylinder(plane, m_radius, m_height);
          var brep = cylinder.ToBrep;
          if (null != brep)
            doc.Objects.AddBrep(brep);
        }

        break;
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    private GumballAppearanceSettings RadiusGumballAppearanceSettings()
    {
      var gas = new GumballAppearanceSettings
      {
        RelocateEnabled = false,
        RotateXEnabled = false,
        RotateYEnabled = false,
        RotateZEnabled = false,
        ScaleXEnabled = false,
        ScaleYEnabled = false,
        ScaleZEnabled = false,
        TranslateXEnabled = false,
        TranslateXYEnabled = false,
        TranslateYEnabled = false,
        TranslateYZEnabled = false,
        TranslateZEnabled = true,
        TranslateZXEnabled = false,
        MenuEnabled = false
      };
      return gas;
    }

    private GumballAppearanceSettings HeightGumballAppearanceSettings()
    {
      var gas = new GumballAppearanceSettings
      {
        RelocateEnabled = false,
        RotateXEnabled = false,
        RotateYEnabled = false,
        RotateZEnabled = false,
        ScaleXEnabled = false,
        ScaleYEnabled = false,
        ScaleZEnabled = false,
        TranslateXEnabled = false,
        TranslateXYEnabled = false,
        TranslateYEnabled = false,
        TranslateYZEnabled = false,
        TranslateZEnabled = true,
        TranslateZXEnabled = false,
        MenuEnabled = false
      };
      return gas;
    }
  }


  /// <summary>
  /// SampleCsGumballCylinder
  /// </summary>
  class SampleCsGumballCylinder
  {
    readonly double m_radius;
    readonly double m_height;
    Circle m_circle;
    Cylinder m_cylinder;
    Brep m_brep;

    public SampleCsGumballCylinder()
    {
      if (Create(Plane.WorldXY, 1.0, 1.0))
      {
        m_radius = 1.0;
        m_height = 1.0;
      }
    }

    public SampleCsGumballCylinder(Plane plane, double radius, double height)
    {
      if (Create(plane, radius, height))
      {
        m_radius = radius;
        m_height = height;
      }
    }

    public bool Create(Plane plane, double radius, double height)
    {
      var rc = false;
      if (radius > 0.0 && height > 0.0)
      {
        m_circle = new Circle(plane, radius);
        m_cylinder = new Cylinder(m_circle, height);
        rc = m_cylinder.IsValid;
        if (rc)
        {
          m_brep = ToBrep;
          rc = m_brep.IsValid;
        }
      }
      return rc;
    }

    public Point3d Center => m_circle.Center;

    public double Radius
    {
      get => m_circle.Radius;
      set => Create(m_circle.Plane, value, m_cylinder.TotalHeight);
    }

    public double Height
    {
      get => m_cylinder.TotalHeight;
      set => Create(m_circle.Plane, m_circle.Radius, value);
    }

    public Plane RadiusPlane
    {
      get
      {
        var center = m_circle.PointAt(0.0);
        var xaxis = m_circle.TangentAt(0.0);
        xaxis.Unitize();
        var zaxis = m_circle.Normal;
        zaxis.Unitize();
        return new Plane(center, xaxis, zaxis);
      }
    }

    public Plane HeightPlane
    {
      get
      {
        var dir = m_circle.Normal;
        dir.Unitize();
        dir *= m_cylinder.TotalHeight;
        var center = Point3d.Add(m_circle.Center, dir);
        return new Plane(center, m_circle.Plane.XAxis, m_circle.Plane.YAxis);
      }
    }

    public double BaseRadius => m_radius;

    public double BaseHeight => m_height;

    public void Draw(DisplayPipeline display)
    {
      if (null != m_brep)
        display.DrawBrepWires(m_brep, Rhino.ApplicationSettings.AppearanceSettings.FeedbackColor);
    }

    public Brep ToBrep
    {
      get
      {
        if (m_cylinder.IsValid)
          return m_cylinder.ToBrep(true, true);
        return null;
      }
    }
  }


  /// <summary>
  /// SampleCsGumballCylinderGetPoint
  /// </summary>
  class SampleCsGumballCylinderGetPoint : GetPoint
  {
    private readonly SampleCsGumballCylinder m_cylinder;
    private readonly GumballDisplayConduit m_radius_dc;
    private readonly GumballDisplayConduit m_height_dc;
    private Point3d m_base_origin;
    private Point3d m_base_point;

    public SampleCsGumballCylinderGetPoint(SampleCsGumballCylinder cylinder, GumballDisplayConduit radiusDc, GumballDisplayConduit heightDc)
    {
      m_cylinder = cylinder;
      m_radius_dc = radiusDc;
      m_height_dc = heightDc;
      m_base_origin = Point3d.Unset;
      m_base_point = Point3d.Unset;
    }

    protected override void OnMouseDown(GetPointMouseEventArgs e)
    {
      if (m_radius_dc.PickResult.Mode != GumballMode.None || m_height_dc.PickResult.Mode != GumballMode.None)
        return;

      m_base_origin = Point3d.Unset;
      m_base_point = Point3d.Unset;

      m_radius_dc.PickResult.SetToDefault();
      m_height_dc.PickResult.SetToDefault();

      var pick_context = new PickContext
      {
        View = e.Viewport.ParentView,
        PickStyle = PickStyle.PointPick
      };

      var xform = e.Viewport.GetPickTransform(e.WindowPoint);
      pick_context.SetPickTransform(xform);

      Line pick_line;
      e.Viewport.GetFrustumLine(e.WindowPoint.X, e.WindowPoint.Y, out pick_line);

      pick_context.PickLine = pick_line;
      pick_context.UpdateClippingPlanes();

      // try picking one of the gumballs
      if (m_radius_dc.PickGumball(pick_context, this))
      {
        m_base_origin = m_cylinder.RadiusPlane.Origin;
        m_base_point = e.Point;
      }
      else if (m_height_dc.PickGumball(pick_context, this))
      {
        m_base_origin = m_cylinder.HeightPlane.Origin;
        m_base_point = e.Point;
      }
    }

    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      if (m_base_origin.IsValid && m_base_point.IsValid)
      {
        Line world_line;
        if (e.Viewport.GetFrustumLine(e.WindowPoint.X, e.WindowPoint.Y, out world_line))
        {
          var dir = e.Point - m_base_point;
          var len = dir.Length;
          if (m_base_origin.DistanceTo(e.Point) < m_base_origin.DistanceTo(m_base_point))
            len = -len;

          if (m_radius_dc.PickResult.Mode != GumballMode.None)
          {
            // update radius gumball
            m_radius_dc.UpdateGumball(e.Point, world_line);
            // update cylinder
            m_cylinder.Radius = m_cylinder.BaseRadius + len;
          }
          else if (m_height_dc.PickResult.Mode != GumballMode.None)
          {
            // update hight gumball
            m_height_dc.UpdateGumball(e.Point, world_line);
            // update cylinder
            m_cylinder.Height = m_cylinder.BaseHeight + len;
          }
        }
      }

      base.OnMouseMove(e);
    }

    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      m_cylinder.Draw(e.Display);

      // Disable default GetPoint drawing by not calling the base class
      // implementation. All aspects of gumball display are handled by 
      // GumballDisplayConduit
      //base.OnDynamicDraw(e);
    }

    public GetResult MoveGumball()
    {
      var rc = Get(true);
      return rc;
    }
  }
}
