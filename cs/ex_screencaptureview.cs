using System;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;

namespace examples_cs
{
  public class CaptureViewToBitmapCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csCaptureViewToBitmap"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var file_name = "";

      var bitmap = doc.Views.ActiveView.CaptureToBitmap(true, true, true);
      bitmap.MakeTransparent();

      // copy bitmap to clipboard
      Clipboard.SetImage(bitmap);

      // save bitmap to file
      var save_file_dialog = new Rhino.UI.SaveFileDialog
      {
        Filter = "*.bmp",
        InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)
      };
      if (save_file_dialog.ShowDialog() == DialogResult.OK)
      {
        file_name = save_file_dialog.FileName;
      }

      if (file_name != "")
        bitmap.Save(file_name);

      return Rhino.Commands.Result.Success;
    }
  }
}
