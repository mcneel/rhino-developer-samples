using Rhino;
using Rhino.Commands;
using System;
using System.Runtime.InteropServices;

namespace SampleCsCommands
{
  public class SampleCsQuit : Command
  {
    public override string EnglishName => "SampleCsQuit";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      doc.Modified = false;
      PostMessage(RhinoApp.MainWindowHandle(), 0x0010 /*WM_CLOSE*/, 0, 0);
      return Result.Success;
    }

    [DllImport("User32.Dll")]
    private static extern bool PostMessage(IntPtr hWnd, uint msg, int wParam, int lParam);
  }
}