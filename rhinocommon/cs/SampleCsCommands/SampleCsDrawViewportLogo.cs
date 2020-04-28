using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsDrawViewportLogo command
  /// </summary>
  public class SampleCsDrawViewportLogo : Command
  {
    public override string EnglishName => "SampleCsDrawViewportLogo";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      SampleCsDrawViewportLogoConduit conduit = new SampleCsDrawViewportLogoConduit();
      conduit.Enabled = true;
      doc.Views.Redraw();

      GetString gs = new GetString();
      gs.SetCommandPrompt("Press <Enter> to continue");
      gs.AcceptNothing(true);
      gs.Get();

      conduit.Enabled = false;
      doc.Views.Redraw();

      return Result.Success;
    }
  }

  /// <summary>
  /// SampleCsDrawViewportLogoConduit display conduit
  /// </summary>
  public class SampleCsDrawViewportLogoConduit : Rhino.Display.DisplayConduit
  {
    private float m_sprite_size = 64;
    private System.Drawing.Bitmap m_logo;
    private readonly Rhino.Display.DisplayBitmap m_bitmap;
    
    public SampleCsDrawViewportLogoConduit()
    {
      m_logo = SampleCsCommands.Properties.Resources.Logo;
      m_bitmap = new Rhino.Display.DisplayBitmap(m_logo);
    }

    protected override void DrawForeground(Rhino.Display.DrawEventArgs e)
    {
      System.Drawing.Rectangle rect = e.Viewport.Bounds;
      Point2d point = new Point2d(rect.Right - (0.5 * m_sprite_size), rect.Bottom - (0.5 * m_sprite_size));
      e.Display.DrawSprite(m_bitmap, point, m_sprite_size);
    }
  }

}
