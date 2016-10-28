partial class Examples
{
  public static Rhino.Commands.Result ShowSurfaceDirection(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef objref;
    var rc = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface for direction display",
      false,
      Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.PolysrfFilter,
      out objref);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    var brep = objref.Brep();
    if (brep == null)
      return Rhino.Commands.Result.Failure;

    bool bIsSolid = brep.IsSolid;

    TestSurfaceDirConduit conduit = new TestSurfaceDirConduit(brep);
    conduit.Enabled = true;
    doc.Views.Redraw();

    var gf = new Rhino.Input.Custom.GetOption();
    gf.SetCommandPrompt("Press enter when done");
    gf.AcceptNothing(true);
    if (!bIsSolid)
      gf.AddOption("Flip");

    for (; ; )
    {
      var res = gf.Get();
      if (res == Rhino.Input.GetResult.Option)
      {
        conduit.Flip = !conduit.Flip;
        doc.Views.Redraw();
        continue;
      }
      if (res == Rhino.Input.GetResult.Nothing)
      {
        if (!bIsSolid && conduit.Flip)
        {
          brep.Flip();
          doc.Objects.Replace(objref, brep);
        }
      }
      break;
    }

    conduit.Enabled = false;
    doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}

class TestSurfaceDirConduit : Rhino.Display.DisplayConduit
{
  readonly Brep m_brep;
  readonly List<Point3d> m_points;
  readonly List<Vector3d> m_normals;

  public TestSurfaceDirConduit(Brep brep)
  {
    m_brep = brep;
    Flip = false;

    const int SURFACE_ARROW_COUNT = 5;
    int face_count = m_brep.Faces.Count;
    int capacity = face_count * SURFACE_ARROW_COUNT * SURFACE_ARROW_COUNT;
    m_points = new List<Point3d>(capacity);
    m_normals = new List<Vector3d>(capacity);

    for (int i = 0; i < face_count; i++)
    {
      var face = brep.Faces[i];
      var loop = face.OuterLoop;
      if (loop == null)
        continue;

      var udomain = face.Domain(0);
      var vdomain = face.Domain(1);
      var loop_bbox = loop.GetBoundingBox(true);
      if (loop_bbox.IsValid)
      {
        Interval domain = new Interval(loop_bbox.Min.X, loop_bbox.Max.X);
        domain = Interval.FromIntersection(domain, udomain);
        if (domain.IsIncreasing)
          udomain = domain;
        domain = new Interval(loop_bbox.Min.Y, loop_bbox.Max.Y);
        domain = Interval.FromIntersection(domain, vdomain);
        if (domain.IsIncreasing)
          vdomain = domain;
      }

      bool bUntrimmed = face.IsSurface;
      bool bRev = face.OrientationIsReversed;
      for (double u = 0; u < SURFACE_ARROW_COUNT; u += 1.0)
      {
        double d = u / (SURFACE_ARROW_COUNT - 1.0);
        double s = udomain.ParameterAt(d);

        var intervals = face.TrimAwareIsoIntervals(1, s);
        if (bUntrimmed || intervals.Length > 0)
        {
          for (double v = 0; v < SURFACE_ARROW_COUNT; v += 1.0)
          {
            d = v / (SURFACE_ARROW_COUNT - 1.0);
            double t = vdomain.ParameterAt(d);
            bool bAdd = bUntrimmed;
            for (int k = 0; !bAdd && k < intervals.Length; k++)
            {
              if (intervals[k].IncludesParameter(t))
                bAdd = true;
            }
            if (bAdd)
            {
              var pt = face.PointAt(s, t);
              var vec = face.NormalAt(s, t);
              m_points.Add(pt);
              if (bRev)
                vec.Reverse();
              m_normals.Add(vec);
            }
          }
        }
      }
    }
  }

  public bool Flip { get; set; }

  protected override void DrawOverlay(Rhino.Display.DrawEventArgs e)
  {
    if (m_points.Count > 0)
    {
      var color = Rhino.ApplicationSettings.AppearanceSettings.TrackingColor;
      for (int i = 0; i < m_points.Count; i++)
      {
        if (i % 100 == 0 && e.Display.InterruptDrawing())
          break;

        var pt = m_points[i];
        var dir = m_normals[i];
        if (Flip)
          dir.Reverse();
        e.Display.DrawDirectionArrow(pt, dir, color);
      }
    }
  }
}
