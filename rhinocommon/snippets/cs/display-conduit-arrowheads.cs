class DrawArrowHeadsConduit : Rhino.Display.DisplayConduit
{
  private readonly Line m_line;
  private readonly int m_screen_size;
  private readonly double m_world_size;

  public DrawArrowHeadsConduit(Line line, int screenSize, double worldSize)
  {
    m_line = line;
    m_screen_size = screenSize;
    m_world_size = worldSize;
  }

  protected override void DrawForeground(Rhino.Display.DrawEventArgs e)
  {
    e.Display.DrawArrow(m_line, System.Drawing.Color.Black, m_screen_size, m_world_size);
  }
}

partial class Examples
{
  static DrawArrowHeadsConduit m_draw_conduit;

  public static Result ConduitArrowHeads(RhinoDoc doc)
  {
    if (m_draw_conduit != null)
    {
      RhinoApp.WriteLine("Turn off existing arrowhead conduit");
      m_draw_conduit.Enabled = false;
      m_draw_conduit = null;
    }
    else
    {
      // get arrow head size
      var go = new GetOption();
      go.SetCommandPrompt("ArrowHead length in screen size (pixels) or world size (percentage of arrow length)?");
      go.AddOption("screen");
      go.AddOption("world");
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      int screen_size = 0;
      double world_size = 0.0;
      if (go.Option().EnglishName == "screen")
      {
        var gi = new GetInteger();
        gi.SetLowerLimit(0, true);
        gi.SetCommandPrompt("Length of arrow head in pixels");
        gi.Get();
        if (gi.CommandResult() != Result.Success)
          return gi.CommandResult();
        screen_size = gi.Number();
      }
      else
      {
        var gi = new GetInteger();
        gi.SetLowerLimit(0, true);
        gi.SetUpperLimit(100, false);
        gi.SetCommandPrompt("Length of arrow head in percentage of total arrow length");
        gi.Get();
        if (gi.CommandResult() != Result.Success)
          return gi.CommandResult();
        world_size = gi.Number() / 100.0;
      }


      // get arrow start and end points
      var gp = new GetPoint();
      gp.SetCommandPrompt("Start of line");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();
      var start_point = gp.Point();

      gp.SetCommandPrompt("End of line");
      gp.SetBasePoint(start_point, false);
      gp.DrawLineFromPoint(start_point, true);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();
      var end_point = gp.Point();

      var v = end_point - start_point;
      if (v.IsTiny(Rhino.RhinoMath.ZeroTolerance))
        return Result.Nothing;

      var line = new Line(start_point, end_point);

      m_draw_conduit = new DrawArrowHeadsConduit(line, screen_size, world_size);
      // toggle conduit on/off
      m_draw_conduit.Enabled = true;
      RhinoApp.WriteLine("Draw arrowheads conduit enabled.");
    }
    doc.Views.Redraw();
    return Result.Success;
  }
}
