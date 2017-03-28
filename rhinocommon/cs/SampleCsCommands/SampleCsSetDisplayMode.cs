using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("fe532160-4567-4821-a59c-d1334fc7a371")]
  public class SampleCsSetDisplayMode : Command
  {
    public SampleCsSetDisplayMode()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsSetDisplayMode"; }
    }

    /// <summary>
    /// Remove characters that are invalid for command options display
    /// </summary>
    static string FormatValidDisplayName(string displayName)
    {
      displayName = displayName.Replace("_", "");
      displayName = displayName.Replace(" ", "");
      displayName = displayName.Replace("-", "");
      displayName = displayName.Replace(",", "");
      displayName = displayName.Replace(".", "");
      displayName = displayName.Replace("/", "");
      displayName = displayName.Replace("(", "");
      displayName = displayName.Replace(")", "");
      return displayName;
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Display.RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      Rhino.Display.RhinoViewport viewport = view.ActiveViewport;
      Rhino.Display.DisplayModeDescription currentDisplayMode = viewport.DisplayMode;
      Rhino.RhinoApp.WriteLine("Viewport in {0} display.", currentDisplayMode.EnglishName);

      Rhino.Display.DisplayModeDescription[] displayModes =  Rhino.Display.DisplayModeDescription.GetDisplayModes();

      Rhino.Input.Custom.GetOption go = new Rhino.Input.Custom.GetOption();
      go.SetCommandPrompt("Select new display mode");
      go.AcceptNothing(true);

      foreach (Rhino.Display.DisplayModeDescription displayMode in displayModes)
      {
        string displayName = FormatValidDisplayName(displayMode.EnglishName);
        go.AddOption(displayName);
      }

      Rhino.Input.GetResult rc = go.Get();
      switch (rc)
      {
        case Rhino.Input.GetResult.Option:
          {
            int optionIndex = go.Option().Index;
            if (optionIndex > 0 && optionIndex <= displayModes.Length)
            {
              Rhino.Display.DisplayModeDescription newDisplayMode = displayModes[optionIndex - 1];
              if (newDisplayMode.Id != currentDisplayMode.Id)
              {
                viewport.DisplayMode = newDisplayMode;
                view.Redraw();
                Rhino.RhinoApp.WriteLine("Viewport set to {0} display.", viewport.DisplayMode.EnglishName);
              }
            }
          }
          break;

        case Rhino.Input.GetResult.Cancel:
          return Result.Cancel;

        default:
          break;
      }

      return Result.Success;
    }
  }
}
