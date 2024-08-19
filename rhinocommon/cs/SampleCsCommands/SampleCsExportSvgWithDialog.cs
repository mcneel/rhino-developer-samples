using System;
using Eto.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.UI.Forms;
using Rhino.UI;
using Command = Rhino.Commands.Command;

namespace SampleCsCommands
{
  /// <summary>
  /// Exports the contents of the 3dm file to an svg via the print dialog.
  /// The print dialog is automatically configured for SVG exporting. 
  /// </summary>
  public class SampleCsExportSvgWithDialog : Command
  {
    public override string EnglishName => "SampleCsExportSvgWithDialog";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      
      //Create an ETO file save dialog
      var save_dialog = new Eto.Forms.SaveFileDialog()
      {
        CheckFileExists = false,
        Title = "Export SVG"
      };

      //Add an SVG filter to the save dialog
      save_dialog.Filters.Add(new FileFilter("Export", "*.svg"));
      save_dialog.CurrentFilter = save_dialog.Filters[0];

      //Show the file save dialog
      var result = save_dialog.ShowDialog(RhinoEtoApp.MainWindow);
      if (result != DialogResult.Ok)
        return Result.Cancel;

      //The previous Print dialog settings are stored in the Commands.rhp plug-in. We can get its previous settings directly from the plug-in. 
      Guid.TryParse("02bf604d-799c-4cc2-830e-8d72f21b14b7", out var commands_id);
      
      //Show the Print Dialog configured for SVG export. 
      var svg_export_view_capture_settings = PrintDialogUi.EtoExportSvg(doc.RuntimeSerialNumber, PersistentSettings.FromPlugInId(commands_id));

      if (svg_export_view_capture_settings == null)
        return Result.Cancel;

      //Get the SVG Xml settings from the ViewCapture. 
      var capture_to_svg = Rhino.Display.ViewCapture.CaptureToSvg(svg_export_view_capture_settings);
      if (capture_to_svg == null)
        return Result.Cancel;

      //Save the xml to disk
      capture_to_svg.Save(save_dialog.FileName);


      return Result.Success;
    }
  }
}
