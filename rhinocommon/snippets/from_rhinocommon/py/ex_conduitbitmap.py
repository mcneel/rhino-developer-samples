import Rhino
from Rhino.Geometry import *
import System.Drawing
import Rhino.Display
import scriptcontext
import rhinoscriptsyntax as rs

class CustomConduit(Rhino.Display.DisplayConduit):
    def __init__(self):
      flag = System.Drawing.Bitmap(100,100)
      for x in range(0,100):
        for y in range(0,100):
          flag.SetPixel(x, y, System.Drawing.Color.Red)
      g = System.Drawing.Graphics.FromImage(flag)
      g.FillEllipse(System.Drawing.Brushes.Blue, 25, 25, 50, 50)
      self.display_bitmap = Rhino.Display.DisplayBitmap(flag)

    def DrawForeground(self, e):
      e.Display.DrawBitmap(self.display_bitmap, 50, 50, System.Drawing.Color.Red)

if __name__== "__main__":
    conduit = CustomConduit()
    conduit.Enabled = True
    scriptcontext.doc.Views.Redraw()
    rs.GetString("Pausing for user input")
    conduit.Enabled = False
    scriptcontext.doc.Views.Redraw()