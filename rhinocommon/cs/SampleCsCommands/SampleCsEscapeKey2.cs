using Rhino;
using Rhino.Commands;
using System;

namespace SampleCsCommands
{
  /// <summary>
  /// Escape key event handler helper class
  /// </summary>
  class EscapeKeyEventHandler : IDisposable
  {
    private bool m_escape_key_pressed;

    public EscapeKeyEventHandler(string message)
    {
      RhinoApp.EscapeKeyPressed += RhinoApp_EscapeKeyPressed;
      RhinoApp.WriteLine(message);
    }

    public bool EscapeKeyPressed
    {
      get
      {
        RhinoApp.Wait(); // "pumps" the Rhino message queue
        return m_escape_key_pressed;
      }
    }

    private void RhinoApp_EscapeKeyPressed(object sender, EventArgs e)
    {
      m_escape_key_pressed = true;
    }

    public void Dispose()
    {
      RhinoApp.EscapeKeyPressed -= RhinoApp_EscapeKeyPressed;
    }
  }

  public class SampleCsEscapeKey2 : Command
  {
    private readonly Random m_random;

    public SampleCsEscapeKey2()
    {
      m_random = new Random();
    }

    public override string EnglishName => "SampleCsEscapeKey2";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      EscapeKeyEventHandler handler = new EscapeKeyEventHandler("Press <Esc> to cancel");

      for (int i = 0; i < 1000; i++)
      {
        if (handler.EscapeKeyPressed)
        {
          RhinoApp.WriteLine("Command canceled");
          break;
        }

        double x = (double)m_random.Next(0, 100);
        double y = (double)m_random.Next(0, 100);
        double z = (double)m_random.Next(0, 100);
        Rhino.Geometry.Point3d pt = new Rhino.Geometry.Point3d(x, y, z);
        doc.Objects.AddPoint(pt);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
