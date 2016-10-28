from scriptcontext import doc
from System.Windows.Forms import *
import Rhino.UI
from System import Environment

def RunCommand():
  file_name = "";

  bitmap = doc.Views.ActiveView.CaptureToBitmap(True, True, True)

  # copy bitmap to clipboard
  Clipboard.SetImage(bitmap)


  # save bitmap to file
  save_file_dialog = Rhino.UI.SaveFileDialog()
  save_file_dialog.Filter = "*.bmp"
  save_file_dialog.InitialDirectory = \
    Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)

  if save_file_dialog.ShowDialog() == DialogResult.OK:
    file_name = save_file_dialog.FileName

  if file_name != "":
    bitmap.Save(file_name)

  return Rhino.Commands.Result.Success

if __name__ == "__main__":
  RunCommand()