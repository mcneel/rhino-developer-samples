partial class Examples
{
  static MyConduit m_conduit;
  public static Result DisplayConduit(RhinoDoc doc)
  {
    // The following code lets you toggle the conduit on and off by repeatedly running the command
    if (m_conduit != null)
    {
      m_conduit.Enabled = false;
      m_conduit = null;
    }
    else
    {
      m_conduit = new MyConduit { Enabled = true };
    }
    doc.Views.Redraw();
    return Result.Success;
  }
}

class MyConduit : Rhino.Display.DisplayConduit
{
  protected override void CalculateBoundingBox(CalculateBoundingBoxEventArgs e)
  {
    base.CalculateBoundingBox(e);
    e.IncludeBoundingBox(e.Display.Viewport.ConstructionPlane().Origin);
  }

  protected override void PreDrawObjects(DrawEventArgs e)
  {
    base.PreDrawObjects(e);

    var c_plane = e.Display.Viewport.ConstructionPlane();
    var x_color = Rhino.ApplicationSettings.AppearanceSettings.GridXAxisLineColor;
    var y_color = Rhino.ApplicationSettings.AppearanceSettings.GridYAxisLineColor;
    var z_color = Rhino.ApplicationSettings.AppearanceSettings.GridZAxisLineColor;

    e.Display.PushDepthWriting(false);
    e.Display.PushDepthTesting(false);

    e.Display.DrawPoint(c_plane.Origin, System.Drawing.Color.White);
    e.Display.DrawArrow(new Line(c_plane.Origin, new Vector3d(c_plane.XAxis) * 10.0), x_color);
    e.Display.DrawArrow(new Line(c_plane.Origin, new Vector3d(c_plane.YAxis) * 10.0), y_color);
    e.Display.DrawArrow(new Line(c_plane.Origin, new Vector3d(c_plane.ZAxis) * 10.0), z_color);

    e.Display.PopDepthWriting();
    e.Display.PopDepthTesting();
  }
}
