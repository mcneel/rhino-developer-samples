import Rhino
import rhinoscriptsyntax as rs
from scriptcontext import doc

import clr
clr.AddReference("System.Windows.Forms")
import System.Windows.Forms

def RunCommand():

  fn = rs.OpenFileName(title="select file", filter="Rhino files|*.3dm||")
  if fn == None:
    return

  bitmap = doc.ExtractPreviewImage(fn)

  f = System.Windows.Forms.Form()
  f.Height = bitmap.Height
  f.Width = bitmap.Width
  pb = System.Windows.Forms.PictureBox()
  pb.Image = bitmap
  pb.Height = bitmap.Height  #SizeMode = System.Windows.Forms.PictueBoxSizeMode.AutoSize
  pb.Width = bitmap.Width
  f.Controls.Add(pb);
  f.Show();

if __name__ == "__main__":
  RunCommand()
