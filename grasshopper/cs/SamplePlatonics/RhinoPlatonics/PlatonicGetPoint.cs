using Rhino.Geometry;

namespace RhinoPlatonics
{
  /// <summary>
  /// Custom GetPoint-inherited class used by platonic commands
  /// </summary>
  class PlatonicGetPoint : Rhino.Input.Custom.GetPoint
  {
    Rhino.Geometry.Brep[] m_faces;
    Rhino.Geometry.Point3d m_origin;
    double m_length;
    bool m_draw;
    Rhino.Geometry.Transform m_xform;

    /// <summary>
    /// Constructor
    /// </summary>
    public PlatonicGetPoint(Rhino.Geometry.Brep[] faces, Rhino.Geometry.Point3d origin)
    {
      m_faces = faces;
      m_origin = origin;

      m_length = 0.0;
      m_draw = false;

      this.MouseMove += new System.EventHandler<Rhino.Input.Custom.GetPointMouseEventArgs>(PlatonicGetPoint_MouseMove);
      this.DynamicDraw += new System.EventHandler<Rhino.Input.Custom.GetPointDrawEventArgs>(PlatonicGetPoint_DynamicDraw);
    }

    public double Length
    {
      get { return m_length; }
    }

    /// <summary>
    /// Transform calculator
    /// </summary>
    public bool CalculateTransform(Rhino.Display.RhinoViewport vp, Rhino.Geometry.Point3d pt, ref Rhino.Geometry.Transform xform)
    {
      bool rc = false;

      if (null == xform)
        xform = new Rhino.Geometry.Transform();
      xform = Rhino.Geometry.Transform.Identity;

      if( pt.IsValid )
      {
        Rhino.Geometry.Plane plane = new Rhino.Geometry.Plane(vp.ConstructionPlane()) {Origin = m_origin};

        Rhino.Geometry.Vector3d xaxis = (pt - plane.Origin);
        double length = xaxis.Length;

        if (length >= Rhino.RhinoMath.ZeroTolerance)
        {
          Rhino.Geometry.Vector3d yaxis;
          yaxis = 0 != xaxis.IsParallelTo(plane.Normal) ? plane.YAxis : Rhino.Geometry.Vector3d.CrossProduct(plane.Normal, xaxis);

          plane = new Rhino.Geometry.Plane(plane.Origin, xaxis, yaxis);
          if (!plane.IsValid) return rc;

          Rhino.Geometry.Transform rotate_xform = Rhino.Geometry.Transform.PlaneToPlane(Rhino.Geometry.Plane.WorldXY, plane);
          Rhino.Geometry.Transform scale_xform = Rhino.Geometry.Transform.Scale(plane, length, length, length);
            
          xform = scale_xform * rotate_xform;
          rc = xform.IsValid;

          if( rc )
            m_length = length;
        }
      }

      return rc;
    }

    /// <summary>
    /// Transform calculator
    /// </summary>
    public bool CalculateTransform(Rhino.Display.RhinoViewport vp, double length, ref Rhino.Geometry.Transform xform)
    {
      bool rc = false;

      if (null == xform)
        xform = new Rhino.Geometry.Transform();
      xform = Rhino.Geometry.Transform.Identity;

      if (length >= Rhino.RhinoMath.ZeroTolerance)
      {
        Rhino.Geometry.Plane plane = new Rhino.Geometry.Plane(vp.ConstructionPlane()) {Origin = m_origin};

        Rhino.Geometry.Transform rotate_xform = Rhino.Geometry.Transform.PlaneToPlane(Rhino.Geometry.Plane.WorldXY, plane);
        Rhino.Geometry.Transform scale_xform = Rhino.Geometry.Transform.Scale(plane, length, length, length);
    
        xform = scale_xform * rotate_xform;
        rc = xform.IsValid;

        if( rc )
          m_length = length;
      }

      return rc;
    }

    /// <summary>
    /// Mouse move event handler
    /// </summary>
    void PlatonicGetPoint_MouseMove(object sender, Rhino.Input.Custom.GetPointMouseEventArgs e)
    {
      m_draw = CalculateTransform(e.Viewport, e.Point, ref m_xform);
    }

    /// <summary>
    /// Dynamic draw event handler
    /// </summary>
    void PlatonicGetPoint_DynamicDraw(object sender, Rhino.Input.Custom.GetPointDrawEventArgs e)
    {
      if (m_draw)
      {
        if (null != m_faces && null != m_xform)
        {
          Rhino.Geometry.Transform model_xform = e.Display.ModelTransform;
          e.Display.ModelTransform = m_xform;

          System.Drawing.Color color = e.RhinoDoc.Layers.CurrentLayer.Color;
          foreach (Brep t in m_faces)
          {
            if (null != t)
              e.Display.DrawBrepWires(t, color);
          }

          e.Display.ModelTransform = model_xform;
        }
      }
    }
  }
}
