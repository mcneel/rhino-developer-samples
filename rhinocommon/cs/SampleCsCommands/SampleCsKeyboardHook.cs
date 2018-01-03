using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsKeyboardHook : Command
  {
    // For a listing of Windows virtual key code, see:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
    private const int VK_DELETE = 0x2E;

    private bool m_enabled;

    public override string EnglishName => "SampleCsKeyboardHook";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      if (Rhino.Runtime.HostUtils.RunningOnOSX)
      {
        RhinoApp.WriteLine("This sample works on Rhino for Windows.");
        return Result.Cancel;
      }

      // Toggle enabled state
      m_enabled = !m_enabled;

      if (m_enabled)
      {
        RhinoApp.KeyboardEvent += OnRhinoKeyboardEvent;
        RhinoApp.WriteLine("Keyboard hook enabled.");
      }
      else
      {
        RhinoApp.KeyboardEvent -= OnRhinoKeyboardEvent;
        RhinoApp.WriteLine("Keyboard hook disabled.");
      }

      return Result.Success;
    }

    /// <summary>
    /// Keyboard event handler
    /// </summary>
    /// <param name="key">The virtual key code.</param>
    private static void OnRhinoKeyboardEvent(int key)
    {
      if (key == VK_DELETE)
        RhinoApp.WriteLine("Delete key pressed");
    }
  }
}