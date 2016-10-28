class CustomConduit : Rhino.Display.DisplayConduit
{
  protected override void DrawForeground(Rhino.Display.DrawEventArgs e)
  {
    var bounds = e.Viewport.Bounds;
    var pt = new Rhino.Geometry.Point2d(bounds.Right - 100, bounds.Bottom - 30);
    e.Display.Draw2dText("Hello", System.Drawing.Color.Red, pt, false);
  }
}

partial class Examples
{
  readonly static CustomConduit m_customconduit = new CustomConduit();
  public static Rhino.Commands.Result DrawOverlay(RhinoDoc doc)
  {
    // toggle conduit on/off
    m_customconduit.Enabled = !m_conduit.Enabled;

    RhinoApp.WriteLine("Custom conduit enabled = {0}", m_customconduit.Enabled);
    doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
