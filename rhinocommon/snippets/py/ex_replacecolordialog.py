from Rhino import *
from Rhino.Commands import *
from Rhino.UI import *
from System.Windows.Forms import *

m_dlg = None

def RunCommand():
  Dialogs.SetCustomColorDialog(OnSetCustomColorDialog)
  return Result.Success

def OnSetCustomColorDialog(sender, e):
  m_dlg = ColorDialog()
  if m_dlg.ShowDialog(None) == DialogResult.OK:
    c = m_dlg.Color
    e.SelectedColor = c

if __name__ == "__main__":
  RunCommand()