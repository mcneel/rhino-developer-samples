using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("9c581028-0837-4f26-a7a0-4465d2975ad9")]
  public class SampleCsEscapeKey : Command
  {
    private bool m_escape_key_pressed;

    public SampleCsEscapeKey()
    {
      m_escape_key_pressed = false;
    }

    public override string EnglishName
    {
      get { return "SampleCsEscapeKey"; }
    }

    void RhinoApp_EscapeKeyPressed(object sender, EventArgs e)
    {
      m_escape_key_pressed = true;
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      m_escape_key_pressed = false;

      // Add our escape key event handler
      RhinoApp.EscapeKeyPressed += RhinoApp_EscapeKeyPressed;

      for (var i = 0; i < 10000; i++)
      {
        // Pauses to keep Windows message pump alive
        RhinoApp.Wait();

        // Check out cancel flag
        if (m_escape_key_pressed)
          break;

        RhinoApp.WriteLine(string.Format("Hello {0}", i));
      }

      // Remove it when finished
      RhinoApp.EscapeKeyPressed -= RhinoApp_EscapeKeyPressed;

      return Result.Success;
    }
  }
}
