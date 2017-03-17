################################################################################
# SampleAppearanceColors.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import Rhino.ApplicationSettings as settings

def RGB(val):
    return str.format("(R={0}, G={1}, B={2})", val.R, val.G, val.B)

def PrintHeading(label):
    Rhino.RhinoApp.WriteLine(label)

def PrintColor(label, color):
    Rhino.RhinoApp.WriteLine("\t{0}: {1}", label, RGB(color))

# Prints Rhino's appearance colors to the command line
def SampleAppearanceColors():
    PrintHeading("Command prompt");
    PrintColor("Background", settings.AppearanceSettings.CommandPromptBackgroundColor);
    PrintColor("Text color", settings.AppearanceSettings.CommandPromptTextColor);
    PrintColor("Hover color", settings.AppearanceSettings.CommandPromptBackgroundColor);
    
    PrintHeading("Viewport colors");
    PrintColor("Background", settings.AppearanceSettings.ViewportBackgroundColor);
    PrintColor("Major grid line", settings.AppearanceSettings.GridThickLineColor);
    PrintColor("Minor grid line", settings.AppearanceSettings.GridThinLineColor);
    PrintColor("X-axis line", settings.AppearanceSettings.GridXAxisLineColor);
    PrintColor("Y-axis line", settings.AppearanceSettings.GridYAxisLineColor);
    PrintColor("Z-axis line", settings.AppearanceSettings.GridZAxisLineColor);
    PrintColor("World axis icon X", settings.AppearanceSettings.WorldCoordIconXAxisColor);
    PrintColor("World axis icon Y", settings.AppearanceSettings.WorldCoordIconYAxisColor);
    PrintColor("World axis icon Z", settings.AppearanceSettings.WorldCoordIconZAxisColor);
    PrintColor("Layout", settings.AppearanceSettings.PageviewPaperColor);
    
    PrintHeading("Object display");
    PrintColor("Selected objects", settings.AppearanceSettings.SelectedObjectColor);
    PrintColor("Locked objects", settings.AppearanceSettings.LockedObjectColor);
    PrintColor("New layer", settings.AppearanceSettings.DefaultLayerColor);
    
    PrintHeading("Interface objects");
    PrintColor("Feedback", settings.AppearanceSettings.CrosshairColor);
    PrintColor("Tracking lines", settings.AppearanceSettings.CrosshairColor);
    PrintColor("Crosshair", settings.AppearanceSettings.CrosshairColor);
    
    PrintHeading("Layer dialog box");
    PrintColor("Layout settings background", settings.AppearanceSettings.CurrentLayerBackgroundColor);
    
    PrintHeading("General");
    PrintColor("Window color 1", settings.AppearanceSettings.GetPaintColor(settings.PaintColor.NormalStart));
    PrintColor("Window color 2", settings.AppearanceSettings.GetPaintColor(settings.PaintColor.NormalEnd));
    PrintColor("Window color 3", settings.AppearanceSettings.GetPaintColor(settings.PaintColor.HotStart));
    PrintColor("Window border", settings.AppearanceSettings.GetPaintColor(settings.PaintColor.NormalBorder));
    PrintColor("Window text", settings.AppearanceSettings.GetPaintColor(settings.PaintColor.TextEnabled));
    PrintColor("Active viewport title", settings.AppearanceSettings.GetPaintColor(settings.PaintColor.ActiveViewportTitle));
    PrintColor("Inactive viewport title", settings.AppearanceSettings.GetPaintColor(settings.PaintColor.InactiveViewportTitle));
    
    PrintHeading("Widget colors");
    PrintColor("U-axis", settings.AppearanceSettings.GetWidgetColor(settings.WidgetColor.UAxisColor));
    PrintColor("V-axis", settings.AppearanceSettings.GetWidgetColor(settings.WidgetColor.VAxisColor));
    PrintColor("W-axis", settings.AppearanceSettings.GetWidgetColor(settings.WidgetColor.WAxisColor));
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleAppearanceColors()